#include <ESP32Servo.h>
#include <BluetoothSerial.h>

#define LED_PIN_ROJO 4
#define LED_PIN_VERDE 2
#define SERVO_PIN 12

Servo Motor;
BluetoothSerial SerialBT;
char dato;

const int ANGULO_ABIERTO = 90;
const int ANGULO_CERRADO = 0;
const int VELOCIDAD_SERVO = 20;

void setup() {
   Serial.begin(9600);
   SerialBT.begin("ESP32_Puerta"); // Nombre del dispositivo Bluetooth
   Serial.println("Bluetooth listo");
   
   pinMode(LED_PIN_ROJO, OUTPUT);
   pinMode(LED_PIN_VERDE, OUTPUT);
   
   Motor.attach(SERVO_PIN);
   onOffLeds();   
}

void loop() {
  if (SerialBT.available()) { 
     dato = SerialBT.read(); 
     if (dato == 'a') {
        abrirPuerta();
     } else if (dato == 'c') {
        cerrarPuerta();
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

void abrirPuerta() {
  digitalWrite(LED_PIN_ROJO, HIGH);  // Rojo parpadea durante la apertura
  digitalWrite(LED_PIN_VERDE, LOW);
  
  for (int angulo = ANGULO_CERRADO; angulo <= ANGULO_ABIERTO; angulo++) {
    Motor.write(angulo);
    delay(VELOCIDAD_SERVO);
  }
  
  digitalWrite(LED_PIN_ROJO, LOW);
  digitalWrite(LED_PIN_VERDE, HIGH); // Verde se enciende cuando la puerta está abierta
  Serial.println("Puerta abierta");
}

void cerrarPuerta() {
  digitalWrite(LED_PIN_VERDE, LOW);  // Verde apagado durante el cierre
  digitalWrite(LED_PIN_ROJO, HIGH);

  for (int angulo = ANGULO_ABIERTO; angulo >= ANGULO_CERRADO; angulo--) {
    Motor.write(angulo);
    delay(VELOCIDAD_SERVO);
  }

  digitalWrite(LED_PIN_ROJO, LOW); // Rojo apagado cuando la puerta está cerrada
  Serial.println("Puerta cerrada");
}