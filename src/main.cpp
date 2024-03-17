#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <../Credentials.h>

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

#define MOISTURE_SENSOR_PIN A0

const int dry_threshold = 800;
const int wet_threshold = 450;

unsigned long lastTimeBotRan;
int botRequestDelay = 1000;

int getMoistureValue() {
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  return moistureValue;
}

void handleNewMessages(int newNumMessages) {
  for (int i = 0; i < newNumMessages; i++) {
    String incomingChatID = String(bot.messages[i].chat_id);

    String incomingText = String(bot.messages[i].text);
    String startCommand = "/kelompok_5";
    String statusCommand = "/moisture_status";
    String instructions = "Type " + startCommand + " to display available commands, and " + statusCommand +" to get the latest moisture status.";
    String welcome = "Hello "  + bot.messages[i].from_name + " and welcome to moisture monitor bot.";

  // send message automatically if soil is too dry
  // int moistureValue = getMoistureValue();
  // if (moistureValue < 500) {
  //   String message = String(moistureValue) + ": water your plant!";
  //   bot.sendMessage(CHAT_ID, message);
  // }

    if (incomingText.equalsIgnoreCase(startCommand)) {
      bot.sendMessage(incomingChatID, welcome, "");
      bot.sendMessage(incomingChatID, instructions, "");
    } else if (incomingText.equalsIgnoreCase(statusCommand)) {
      int moistureValue = getMoistureValue();
      String message;

      if (moistureValue <= wet_threshold) {
        message = "soil is too wet";
      } else if (moistureValue > wet_threshold && moistureValue < dry_threshold ) {
        message = "c'est parfait";
      } else {
        message = "soil is dry af man";
      }

      bot.sendMessage(incomingChatID, String(moistureValue) + ": " + message, "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting to WiFi..");
  }

  Serial.println("connected to WiFi");
  Serial.println(WiFi.localIP());
}

void loop() { 
 if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}