#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// ───────────────────────────────────────────────
// CONFIGURACIÓN DE PINES
// ───────────────────────────────────────────────
#define LED_PIN   12   // LED que se enciende por mensaje manual
#define LED_LUZ   2    // LED controlado según valor de luz (LDR)

// ───────────────────────────────────────────────
// VARIABLES GLOBALES
// ───────────────────────────────────────────────
bool encenderLed = false;

// Estructura igual a la del transmisor
typedef struct {
  float tempC;     // Temperatura en °C
  uint8_t luz;     // Luz en %
  uint8_t mov;     // Movimiento (1 o 0)
} DatosSensores;

DatosSensores datosRecibidos;

// ───────────────────────────────────────────────
// CONFIGURACIÓN INICIAL
// ───────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_LUZ, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error iniciando ESP-NOW");
    return;
  }

  Serial.println("✅ Receptor ESP-NOW listo");

  // Callback cuando se reciben datos
  esp_now_register_recv_cb([](const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    // Si solo llega un byte con valor 1 → activar LED manual
    if (len == 1 && data[0] == 1) {
      encenderLed = true;
      return;
    }

    // Si los datos coinciden con la estructura
    if (len == sizeof(DatosSensores)) {
      memcpy(&datosRecibidos, data, sizeof(datosRecibidos));

      // Mostrar datos recibidos
      Serial.println("────────── DATOS RECIBIDOS ──────────");
      Serial.print("Temperatura: "); Serial.print(datosRecibidos.tempC); Serial.println(" °C");
      Serial.print("Luz: "); Serial.print(datosRecibidos.luz); Serial.println(" %");
      Serial.print("Movimiento: "); Serial.println(datosRecibidos.mov ? "Sí" : "No");
      Serial.println("────────────────────────────────────");

      // Si la luz es mayor al 50%, encender LED_LUZ
      if (datosRecibidos.luz > 50) {
        digitalWrite(LED_LUZ, HIGH);
      } else {
        digitalWrite(LED_LUZ, LOW);
      }
    }
  });
}

// ───────────────────────────────────────────────
// BUCLE PRINCIPAL
// ───────────────────────────────────────────────
void loop() {
  // Si se recibió mensaje del botón → encender LED_PIN 5 s
  if (encenderLed) {
    digitalWrite(LED_PIN, HIGH);
    delay(5000);
    digitalWrite(LED_PIN, LOW);
    encenderLed = false;
    Serial.println("💡 Señal de botón recibida");
  }
}