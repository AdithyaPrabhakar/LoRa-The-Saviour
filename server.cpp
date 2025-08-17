#include <WiFi.h>
#include "BluetoothSerial.h"

// --- Bluetooth instance ---
BluetoothSerial SerialBT;

// --- WiFi SoftAP credentials ---
const char *ssid = "ESP32-Network";
const char *password = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize Bluetooth
  SerialBT.begin("ESP32-Server");
  Serial.println("Bluetooth started on ESP32-Server");

  // Start WiFi as SoftAP
  WiFi.softAP(ssid, password);
  Serial.println("WiFi SoftAP started");
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  // Start server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  // --- Handle Bluetooth → WiFi ---
  if (SerialBT.available()) {
    String btData = SerialBT.readStringUntil('\n');
    Serial.print("Received via Bluetooth: ");
    Serial.println(btData);

    client.print(btData);
    Serial.println("Forwarded to WiFi Client: " + btData);
  }

  // --- Handle WiFi → Bluetooth ---
  if (client.connected() && client.available()) {
    String wifiData = client.readStringUntil('\n');
    Serial.print("Received via WiFi: ");
    Serial.println(wifiData);

    SerialBT.print(wifiData);
    Serial.println("Forwarded to Bluetooth: " + wifiData);
  }
}
