#include <ESP32Servo.h>
#include <BluetoothSerial.h>

#define LED_PIN_ROJO 4
#define LED_PIN_VERDE 2
#define SERVO_PIN 12
#define LDR_PIN 34  // Ajusta el pin del LDR
#define UMBRAL_LUZ_BAJA 1500  // Ajusta este valor según tus pruebas

Servo Motor;
BluetoothSerial SerialBT;
char dato;

const int ANGULO_ABIERTO = 90;
const int ANGULO_CERRADO = 0;
const int VELOCIDAD_SERVO = 20;
int estadoPuerta = ANGULO_CERRADO;  // Estado de la puerta

void setup() {
   Serial.begin(9600);
   SerialBT.begin("ESP32_Puerta");
   Serial.println("Bluetooth listo");

   pinMode(LED_PIN_ROJO, OUTPUT);
   pinMode(LED_PIN_VERDE, OUTPUT);
   pinMode(LDR_PIN, INPUT);
   
   Motor.attach(SERVO_PIN);
   onOffLeds();   
}

void loop() {
  if (SerialBT.available()) { 
     dato = SerialBT.read(); 
     if (dato == 'a' && estadoPuerta != ANGULO_ABIERTO) {
        abrirPuerta();
     } else if (dato == 'c' && estadoPuerta != ANGULO_CERRADO) {
        if (luzBaja()) {
          parpadearLedRojo();
        } else {
          cerrarPuerta();
        }
     }
  }
}

void onOffLeds() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN_ROJO, HIGH);
    digitalWrite(LED_PIN_VERDE, HIGH);
    delay(100);
    digitalWrite(LED_PIN_ROJO, LOW);
    digitalWrite(LED_PIN_VERDE, LOW);
    delay(100);
  }
}

bool luzBaja() {
  int valorLuz = analogRead(LDR_PIN);
  Serial.print("Valor LDR: ");
  Serial.println(valorLuz);
  return valorLuz < UMBRAL_LUZ_BAJA;
}

void abrirPuerta() {
  digitalWrite(LED_PIN_ROJO, HIGH);
  digitalWrite(LED_PIN_VERDE, LOW);
  Motor.attach(SERVO_PIN);
  
  for (int angulo = ANGULO_CERRADO; angulo <= ANGULO_ABIERTO; angulo++) {
    Motor.write(angulo);
    delay(VELOCIDAD_SERVO);
  }

  Motor.detach();
  estadoPuerta = ANGULO_ABIERTO;
  
  digitalWrite(LED_PIN_ROJO, LOW);
  digitalWrite(LED_PIN_VERDE, HIGH);
  Serial.println("Puerta abierta");
}

void cerrarPuerta() {
  digitalWrite(LED_PIN_VERDE, LOW);
  digitalWrite(LED_PIN_ROJO, HIGH);
  Motor.attach(SERVO_PIN);

  for (int angulo = ANGULO_ABIERTO; angulo >= ANGULO_CERRADO; angulo--) {
    Motor.write(angulo);
    delay(VELOCIDAD_SERVO);
  }

  Motor.detach();
  estadoPuerta = ANGULO_CERRADO;

  digitalWrite(LED_PIN_ROJO, LOW);
  Serial.println("Puerta cerrada");
}

void parpadearLedRojo() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN_ROJO, HIGH);
    delay(200);
    digitalWrite(LED_PIN_ROJO, LOW);
    delay(200);
  }
  Serial.println("Luz insuficiente para cerrar la puerta");
}
