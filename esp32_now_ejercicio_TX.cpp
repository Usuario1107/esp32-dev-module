#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// ───────────────────────────────────────────────
// CONFIGURACIÓN DE PINES
// ───────────────────────────────────────────────
#define LM35_PIN   32   // Sensor de temperatura LM35
#define LDR_PIN    34   // Sensor de luz (LDR)
#define PIR_PIN    13   // Sensor de movimiento PIR
#define BUTTON_PIN 14   // Botón opcional (para enviar manualmente)

// ───────────────────────────────────────────────
// DIRECCIÓN MAC DEL RECEPTOR
// ───────────────────────────────────────────────
uint8_t receptorAddress[] = {0xEC, 0xE3, 0x34, 0x21, 0x42, 0xD8};
//ARRIBA AÑADIR DIRECCION MAX DEL RX EN
// ───────────────────────────────────────────────
// ESTRUCTURA DE DATOS A ENVIAR
// Se usan tipos pequeños (float + int8_t) para ahorrar espacio
// ───────────────────────────────────────────────
typedef struct {
  float tempC;      // Temperatura en °C (float)
  uint8_t luz;      // Porcentaje de luz (0–100)
  uint8_t mov;      // Movimiento (1 o 0)
} DatosSensores;

DatosSensores datos;

// ───────────────────────────────────────────────
// CALLBACK: confirma si el mensaje se envió bien
// ───────────────────────────────────────────────
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Estado del envío: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "ÉXITO ✅" : "FALLO ❌");
}

// ───────────────────────────────────────────────
// CONFIGURACIÓN INICIAL
// ───────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando transmisión ESP-NOW...");

  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW 😢");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receptorAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al añadir el peer 😢");
    return;
  }

  Serial.println("Configuración completada ✅");
}

// ───────────────────────────────────────────────
// BUCLE PRINCIPAL
// ───────────────────────────────────────────────
void loop() {
  // Lectura del LM35
  int rawLM35 = analogRead(LM35_PIN);
  float voltaje = (rawLM35 * 3.3) / 4095.0;
  float temperatura = voltaje * 100.0; // 10 mV = 1 °C

  // Lectura del LDR (0–4095 → 0–100%)
  int rawLDR = analogRead(LDR_PIN);
  uint8_t porcentajeLuz = map(rawLDR, 0, 4095, 0, 100);

  // Lectura del PIR (0 o 1)
  uint8_t movimiento = digitalRead(PIR_PIN);

  // Cargar los datos en la estructura
  datos.tempC = temperatura;
  datos.luz = porcentajeLuz;
  datos.mov = movimiento;

  // Enviar los datos automáticamente cada 2 segundos
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio > 2000) {
    esp_now_send(receptorAddress, (uint8_t *)&datos, sizeof(datos));

    Serial.println("─────────────────────────────");
    Serial.print("Temperatura: "); Serial.print(datos.tempC); Serial.println(" °C");
    Serial.print("Luz: "); Serial.print(datos.luz); Serial.println(" %");
    Serial.print("Movimiento: "); Serial.println(datos.mov ? "Sí" : "No");
    Serial.println("─────────────────────────────");

    ultimoEnvio = millis();
  }

  // Envío manual con botón
  if (digitalRead(BUTTON_PIN) == LOW) {
    esp_now_send(receptorAddress, (uint8_t *)&datos, sizeof(datos));
    Serial.println("Datos enviados manualmente 🚀");
    delay(500);
  }

  delay(100);
}
