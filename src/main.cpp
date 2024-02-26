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

int getMoistureValue() {
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  return moistureValue;
}

void handleNewMessages(int newNumMessages) {
  for (int i = 0; i < newNumMessages; i++) {
    String incomingChatID = String(bot.messages[i].chat_id);

    if (incomingChatID != CHAT_ID) {
      bot.sendMessage(CHAT_ID, "WHO THE HELL ARE YOU");
      continue;
    }

    String incomingText = String(bot.messages[i].text);

    String instructions = "Type in /moisture_greet to display available commands, and /moisture_status to get the latest moisture status.";
    if (incomingText == "/moisture_greet") {
      String welcome = "Hello "  + bot.messages[i].from_name + " you dumb mf...";

      bot.sendMessage(CHAT_ID, welcome, "");
      bot.sendMessage(CHAT_ID, instructions, "");
    } else if (incomingText == "/moisture_status") {
      int moistureValue = getMoistureValue();

      String message;

      if (moistureValue < 500) {
        message = "soil is dry af man";
      } else if (moistureValue > 500 && moistureValue < 700) {
        message = "c'est parfait";
      } else {
        message = "soil is too wet";
      }

      bot.sendMessage(CHAT_ID, String(moistureValue) + ": " + message, "");
    } else {
      bot.sendMessage(CHAT_ID, "I don't understand you...");
      bot.sendMessage(CHAT_ID, instructions);
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
  delay(100);

  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while(numNewMessages) {
    Serial.println("got response");
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  // send message automatically if soil is too dry
  int moistureValue = getMoistureValue();
  if (moistureValue < 500) {
    String message = String(moistureValue) + ": water your plant!";
    bot.sendMessage(CHAT_ID, message);
  }

  Serial.println("scanning for messages...");
}