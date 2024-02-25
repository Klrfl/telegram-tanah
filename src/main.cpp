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

String getMoistureValue() {
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  String message;

  if (moistureValue < 500) {
    message = "tanaman lu kering anjing";
  } else if (moistureValue > 500 && moistureValue < 700) {
    message = "udah bagus sih";
  } else {
    message = "terlalu basah coy";
  }

  return String(moistureValue) + ": " + message;
}

void handleNewMessages(int newNumMessages) {
  for (int i = 0; i < newNumMessages; i++) {
    String incomingChatID = String(bot.messages[i].chat_id);

    if (incomingChatID != CHAT_ID) {
      bot.sendMessage(CHAT_ID, "WHO THE HELL ARE YOU");
      continue;
    }

    String incomingText = String(bot.messages[i].text);

    if (incomingText == "/start") {
      String welcome = "Halo"  + bot.messages[i].from_name + " anjing...";
      bot.sendMessage(CHAT_ID, welcome, "");
    } else if (incomingText == "/status") {
      String moistureValue = getMoistureValue();
      bot.sendMessage(CHAT_ID, moistureValue, "");
    }  else {
      bot.sendMessage(CHAT_ID, incomingText, "");
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

  Serial.println("scanning for messages...");
}