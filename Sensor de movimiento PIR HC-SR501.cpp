
#include <Arduino.h>

const int pirPin = 13;  // Pin conectado al OUT del PIR
const int ledPin = 2;   // LED para indicar presencia

void setup() {
  pinMode(pirPin, INPUT);     // PIR como entrada digital
  pinMode(ledPin, OUTPUT);    // LED como salida
  Serial.begin(115200);       // Para ver en el monitor serial
}

void loop() {
  int estado = digitalRead(pirPin); // Leer estado del sensor

  if (estado == HIGH) {
    digitalWrite(ledPin, HIGH);     // Encender LED si hay movimiento
    Serial.println("Movimiento detectado");
  } else {
    digitalWrite(ledPin, LOW);      // Apagar LED si no hay movimiento
  }

  delay(100); // Pequeña pausa para evitar rebotes
}