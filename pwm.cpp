#include <Arduino.h>

const int ledPin = 2;         // Pin conectado al LED
const int pwmChannel = 0;     // Canal PWM (0 a 15)
const int freq = 5000;        // Frecuencia en Hz
const int resolution = 8;     // Resolución de 8 bits (0–255)

void setup() {
  // Configurar el canal PWM
  ledcSetup(pwmChannel, freq, resolution);

  // Asociar el canal PWM al pin físico
  ledcAttachPin(ledPin, pwmChannel);
}

void loop() {
  // Aumentar brillo
  for (int ciclo = 0; ciclo <= 255; ciclo += 5) {
    ledcWrite(pwmChannel, ciclo); // Escribir ciclo de trabajo
    delay(20);
  }

  // Disminuir brillo
  for (int ciclo = 255; ciclo >= 0; ciclo -= 5) {
    ledcWrite(pwmChannel, ciclo);
    delay(20);
  }
}