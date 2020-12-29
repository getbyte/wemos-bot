#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "bot.h"
#include "ota.h"
#include "storage.h"
#include "wifi.h"

// Telegram BOT Token (Get from Botfather)
#ifndef BOT_TOKEN
#define BOT_TOKEN "1234567890:ABCDEFGH_ABCDEFGHABCDEFGHABCDEFGHAB"
#endif

const char *host = "core";

Bot *bot;
OTA *ota;

void setup()
{
  // Set up pins
  pinMode(PIN_CAM1, OUTPUT);
  pinMode(PIN_CAM2, OUTPUT);
  pinMode(PIN_MODEM, OUTPUT);
  digitalWrite(PIN_CAM1, HIGH);
  digitalWrite(PIN_CAM2, HIGH);
  digitalWrite(PIN_MODEM, HIGH);

  Serial.begin(115200);
  SPIFFS.begin();
  memset(&approvedChatId[0], 0, 20 * 5);

  // wait for 30 seconds before start, to allow router to be started (if it starts simultaneously with the bot)
  for (int i = 0; i < 30; i++)
    delay(1000);

  // connect to the AP or start Wifi connection portal
  WifiConnect();
  ota = new OTA(host);

  // load allowed users list
  loadSettings(false);

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  bot = new Bot();
  Serial.println("Init complete");
  delay(10000);
  delay(10000);
}

void loop()
{
  bot->processMessages();
  bot->ping("8.8.8.8", 1000*300); // we will ping google DNS each 5 minutes. 
  ota->check();
}
