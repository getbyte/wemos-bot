#include "sensors.h"

sensors::sensors()
{
    dht = 0;
}

sensors::~sensors()
{
    if (dht) 
        delete dht;
}

void sensors::enableDHT(uint8_t pin)
{
    if (dht)
        delete dht;
    dht = new DHT(pin, DHTTYPE);
    dht->begin();
    bot_sensortime = 0;
    data = getDHT();
}

void sensors::enableKey(uint8_t pin)
{
    pinMode(pin, INPUT_PULLUP);
}

DHTValues sensors::getDHT()
{
  if (millis() - bot_sensortime > BOT_SENSOR_TIMEOUT) 
  {
    data.h = dht->readHumidity();
    data.t = dht->readTemperature();
    bot_sensortime = millis();
  }
  return data;
}

bool sensors::getKey(uint8_t pin)
{
    return digitalRead(pin);
}
