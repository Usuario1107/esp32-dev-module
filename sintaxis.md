Pines GPIO
const int pinLed = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
}

void loop() {
  digitalWrite(pinLed, HIGH); // Enciende LED
  delay(500);
  digitalWrite(pinLed, LOW);  // Apaga LED
  delay(500);
}


pinMode(pin, INPUT/OUTPUT/INPUT_PULLUP) → Configura pin.

digitalWrite(pin, HIGH/LOW) → Escribe valor digital.

digitalRead(pin) → Lee valor digital (HIGH/LOW).

3️⃣ PWM en ESP32
const int ledPin = 13;
const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 8;

void setup() {
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(ledPin, pwmChannel);
}

void loop() {
  for(int duty=0; duty<256; duty++){
    ledcWrite(pwmChannel, duty);
    delay(10);
  }
}


ledcSetup(canal, freq, resolucion) → Configura canal PWM.

ledcAttachPin(pin, canal) → Asigna pin al canal.

ledcWrite(canal, duty) → Cambia el ciclo de trabajo (0-2^resolución-1).

4️⃣ Lectura analógica (ADC)
const int adcPin = 34; // Solo pines ADC

void setup() {
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(adcPin); // 0-4095 (12 bits)
  Serial.println(value);
  delay(500);
}


ADC nativo: 12 bits (0-4095).

analogRead(pin) → devuelve valor analógico del pin.

5️⃣ Mapeo de valores (map)
int sensorValue = analogRead(34); // 0-4095
int brightness = map(sensorValue, 0, 4095, 0, 255); // Escala a 0-255
ledcWrite(0, brightness);


map(valor, deMin, deMax, aMin, aMax) → Convierte un valor de un rango a otro.

6️⃣ Salida analógica (DAC)
const int dacPin = 25; // DAC1

void setup() {}

void loop() {
  for(int i=0; i<256; i++){
    dacWrite(dacPin, i); // 0-255
    delay(10);
  }
}


dacWrite(pin, value) → Genera voltaje analógico en pin 25 o 26.

7️⃣ Wi-Fi
#include <WiFi.h>

const char* ssid = "MiRed";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wi-Fi conectado");
  Serial.println(WiFi.localIP());
}

void loop() {}


WiFi.begin(ssid, password) → Conecta a red Wi-Fi.

WiFi.status() → Estado de conexión.

WiFi.localIP() → IP del ESP32.

8️⃣ Bluetooth clásico
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");
}

void loop() {
  if(SerialBT.available()){
    char c = SerialBT.read();
    Serial.print(c);
  }
}


SerialBT.begin(nombre) → Inicializa Bluetooth.

SerialBT.read() y SerialBT.write() → Leer y escribir datos.

9️⃣ I2C
#include <Wire.h>

void setup() {
  Wire.begin(); // SDA=21, SCL=22
}

void loop() {
  Wire.beginTransmission(0x3C); // Dirección del dispositivo
  Wire.write(0x00);             // Comando o registro
  Wire.endTransmission();
  delay(500);
}


Wire.begin() → Inicializa I2C.

Wire.beginTransmission(addr) → Inicia comunicación con dispositivo.

Wire.write(data) → Envía datos.

🔟 SPI
#include <SPI.h>

void setup() {
  SPI.begin();
}

void loop() {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); // CS
  SPI.transfer(0xFF);
  digitalWrite(5, HIGH);
  SPI.endTransaction();
  delay(500);
}


SPI.beginTransaction(SPISettings(clock, bitOrder, mode)) → Configura SPI.

SPI.transfer(data) → Envía/recibe datos.

digitalWrite(CS, LOW/HIGH) → Controla Chip Select.

1️⃣1️⃣ Pines del ESP32

GPIO digitales: 0-39 (algunos especiales)

ADC: 32-39, 0-15

DAC: 25 y 26

PWM: cualquier pin GPIO

I2C: SDA=21, SCL=22

SPI: VSPI/HSPI (pines según placa)

1️⃣2️⃣ Funciones útiles adicionales
// Leer temperatura interna
float temp = temperatureRead(); // ESP32 interno

// Retrasar en milisegundos
delay(500);

// Retrasar sin bloquear loop
unsigned long t = millis(); // Tiempo desde encendido en ms


Con esta guía tienes una referencia completa de la sintaxis del ESP32 en Arduino IDE, incluyendo:

GPIO digital y analógico

PWM y DAC

Wi-Fi y Bluetooth

I2C y SPI

Map, lectura de sensores y otras utilidades