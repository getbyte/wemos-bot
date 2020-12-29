#ifndef __SENSORS_H
#define __SENSORS_H

#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22       // DHT 22  (AM2302)

typedef struct DHTValues {
    float t;
    float h;
} DHTValues;

class sensors {
public:
    sensors();
    ~sensors();
    void enableDHT(uint8_t pin);
    void enableKey(uint8_t pin);
    DHTValues getDHT();
    bool getKey(uint8_t pin);

private:
    const unsigned long BOT_SENSOR_TIMEOUT = 3000; 
    DHT *dht;
    DHTValues data;
    unsigned long bot_sensortime;
};


#endif
