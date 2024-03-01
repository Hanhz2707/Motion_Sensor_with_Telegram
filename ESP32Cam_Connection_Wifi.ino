/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

// ESP32 #2: TCP SERVER + AN LED
#include <WiFi.h>

#define SERVER_PORT 4080

const char* ssid = "Lifi";     // CHANGE TO YOUR WIFI SSID
const char* password = "Cuong's wifi 19"; // CHANGE TO YOUR WIFI PASSWORD

WiFiServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(115200);

  Serial.println("Set up TCP SERVER");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Start listening for a TCP client (from ESP32 #1)
  TCPserver.begin();
}

void loop() {
  // Wait for a TCP client from ESP32 #1:
  WiFiClient client = TCPserver.available();

  if (client) {
    // Read the command from the TCP client:

    String command = client.readStringUntil('\r');
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(command);

    Serial.println();
    //client.stop();
    
  }
}
