#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Hanh";
const char* password = "huyanh2003";
const char* serverAddress = "172.20.10.11"; // CHANGE TO ESP32#2'S IP ADDRESS
const int serverPort = 4080;

// Initialize Telegram BOT
#define BOTtoken "6784144466:AAEYkQGxqGoOIQ7dEmWWZudqXINJcOZbeKY"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "2005843702"

WiFiClient TCPclient; // TCP client for camera server connection
WiFiClientSecure BotClient; // Client for Telegram bot
UniversalTelegramBot bot(BOTtoken, BotClient);

const int motionSensor = 19;  // PIR Motion Sensor
bool motionDetected = false;

// Indicates when motion is detected
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

void setup() {
  Serial.begin(115200);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  BotClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  // Reconnect to the camera server if needed (Arduino #2)
  if (!TCPclient.connected()) {
    Serial.println("\n\nConnection is disconnected");
    TCPclient.stop();
  
    // If not, reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Reconnected to TCP server");
    } else {
      Serial.println("Failed to reconnect to TCP server");
    }

      // Then send message to Telegram bot is motion is detected
      if (motionDetected) {
        bot.sendMessage(CHAT_ID, "Motion detected", "");
        TCPclient.print("Motion detected\r");
        Serial.println("Motion Detected");
        motionDetected = false;
      }
  }
}