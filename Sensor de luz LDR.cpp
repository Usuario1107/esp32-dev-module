#include <Arduino.h>

const int ldrPin = 34; // Pin analógico

void setup() {
  Serial.begin(115200);
}

void loop() {
  int lectura = analogRead(ldrPin); // 0–4095
  int porcentaje = map(lectura, 0, 4095, 0, 100); // 0–100%

  Serial.print("Binario");
  Serial.print(porcentaje);
  Serial.println();

  Serial.print("Luz: ");
  Serial.print(porcentaje);
  Serial.println(" %");

  delay(500);
}