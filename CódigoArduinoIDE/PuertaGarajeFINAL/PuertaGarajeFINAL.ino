
//Librerías
#include <ESP32Servo.h>       // Control de servo con ESP32
#include <BluetoothSerial.h>  // Comunicación Bluetooth

//Definición de Pines y Constantes
#define LED_PIN_ROJO 4
#define LED_PIN_VERDE 2
#define SERVO_PIN 12
#define LDR_PIN 34
#define PULSADOR_PIN 13 //umbral de luz para determinar si las condiciones son de baja luz.

//Variables de Control
Servo Motor;
BluetoothSerial SerialBT;
char dato;

const int ANGULO_ABIERTO = 90;
const int ANGULO_CERRADO = 0;
const int VELOCIDAD_SERVO = 20;
int estadoPuerta = ANGULO_CERRADO; //Controla si la puerta está abierta o cerrada


void setup() {
  //Configura la conexión Bluetooth
  //Configura los pines de los LEDs y el pulsador
  //Muestra el estado inicial de los LEDs 

   Serial.begin(9600);
   SerialBT.begin("ESP32_Puerta"); //Inicia Bluetooth
   Serial.println("Bluetooth listo");

   pinMode(LED_PIN_ROJO, OUTPUT);  // Configura LED rojo como salida
   pinMode(LED_PIN_VERDE, OUTPUT); // Configura LED verde como salida
   pinMode(LDR_PIN, INPUT);        // Configura LDR como entrada
   pinMode(PULSADOR_PIN, INPUT_PULLUP); // Configuración para el pulsador
   
   Motor.attach(SERVO_PIN); // Conecta el servo al pin especificado
   onOffLeds();            // Muestra el estado inicial de LEDs
}


void loop() {
  
  // Verifica si se recibió un dato por Bluetooth y actúa en consecuencia
  if (SerialBT.available()) { 
     dato = SerialBT.read(); 
     if (dato == 'a' && estadoPuerta != ANGULO_ABIERTO) {
        abrirPuerta();
     } else if (dato == 'c' && estadoPuerta != ANGULO_CERRADO) {
        if (luzBaja()) {
          parpadearLedRojo(); // Alerta si la luz es baja
        } else {
          cerrarPuerta();
        }
     }
  }

  //  Detecta si el pulsador ha sido presionado  Detecta si el pulsador ha sido presionado y cambia el estado de la puerta
  if (digitalRead(PULSADOR_PIN) == LOW) { 
    delay(50); // Antirrebote
    if (estadoPuerta == ANGULO_CERRADO) {
      abrirPuerta();
    } else {
      if (luzBaja()) {
        parpadearLedRojo();
      } else {
        cerrarPuerta();
      }
    }
    delay(300); // Para evitar lecturas múltiples del pulsador
  }
}

void onOffLeds() {
  //Función de encendido y apagado rápido de los LEDs al inicio, para indicar que el sistema está listo
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
  //Lee el valor del sensor de luz (LDR) y determina si es menor al umbral, indicando condiciones de baja luz
  int valorLuz = analogRead(LDR_PIN);
  Serial.print("Valor LDR: ");
  Serial.println(valorLuz);
  return valorLuz < UMBRAL_LUZ_BAJA;
}

void abrirPuerta() {
  //Abre la puerta cambiando el ángulo del servo de ANGULO_CERRADO a ANGULO_ABIERTO, 
  //actualiza el estado de la puerta, y enciende el LED verde
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
  //Similar a abrirPuerta, pero en sentido inverso. Cierra la puerta al pasar el servo de ANGULO_ABIERTO a ANGULO_CERRADO y enciende el LED rojo
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
  //Esta función enciende y apaga el LED rojo tres veces cuando el sensor detecta poca luz y se intenta cerrar la puerta. 
  //Envía un mensaje de advertencia
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN_ROJO, HIGH);
    delay(200);
    digitalWrite(LED_PIN_ROJO, LOW);
    delay(200);
  }
  Serial.println("Luz insuficiente para cerrar la puerta");
}

