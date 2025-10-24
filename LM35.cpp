#include <Arduino.h>
const int lm35Pin = 34; // Pin ADC conectado al LM35

void setup() {
  Serial.begin(115200);
}

void loop() {
  int rawValue = analogRead(lm35Pin); // Lectura ADC (0-4095)
  
  // Convertir a voltaje (3.3V)
  float voltage = (rawValue / 4095.0) * 3.3; 
  
  // Convertir a temperatura en °C
  float tempC = voltage * 100.0; // 10 mV por °C → 3.3V ≈ 33°C máximo

  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.println(" °C");

  delay(1000);
}
