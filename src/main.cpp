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
int moistureValue;

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

int getMoistureValue();

void setup() {
  Serial.begin(115200);
}

void loop() {
  moistureValue = getMoistureValue();
  Serial.println("Moisture value: ");
  Serial.println(moistureValue);

  if (moistureValue < 500) {
    Serial.println("tanaman lu kering anjing");
  } else if (moistureValue > 500 && moistureValue < 700) {
    Serial.println("udah bagus sih");
  } else {
    Serial.println("terlalu basah coy");
  }

  // so we don't get output every millisecond
  delay(5000);
}

int getMoistureValue() {
  int val = analogRead(MOISTURE_SENSOR_PIN);
  return val;
}

void handleNewMessages(int newNumMessages) {
  Serial.println("handling message");
  Serial.println(String(newNumMessages));
}