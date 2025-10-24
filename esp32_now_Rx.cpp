#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define LED_PIN 12
bool encenderLed = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error iniciando ESP-NOW");
    return;
  }

  esp_now_register_recv_cb([](const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if (len == 1 && data[0] == 1) encenderLed = true;
  });
}

void loop() {
  if (encenderLed) {
    digitalWrite(LED_PIN, HIGH);
    delay(5000);
    digitalWrite(LED_PIN, LOW);
    encenderLed = false;
    Serial.println("Mensaje recibido");
  }
}