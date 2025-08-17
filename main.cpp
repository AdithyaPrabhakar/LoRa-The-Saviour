#include <WiFi.h>
#include "BluetoothSerial.h"

// --- Bluetooth instance ---
BluetoothSerial SerialBT;

// --- WiFi Station credentials ---
const char *ssid     = "ESP32-Network";   // Replace with AP ESP32 SSID
const char *password = "12345678";        // Replace with AP ESP32 password

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Initialize Bluetooth
  SerialBT.begin("ESP32-Client");
  Serial.println("Bluetooth started on ESP32-Client");

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("Client IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Ensure client is connected to the server (Module 1 / SoftAP ESP32)
  if (!client.connected()) {
    client.connect(WiFi.gatewayIP(), 80); // Replace with server IP if known
  }

  // --- Handle Bluetooth → WiFi ---
  if (SerialBT.available()) {
    String btData = SerialBT.readStringUntil('\n');
    Serial.print("Received via Bluetooth: ");
    Serial.println(btData);

    if (client.connected()) {
      client.print(btData);
      Serial.println("Forwarded to WiFi: " + btData);
    }
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
