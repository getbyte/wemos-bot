#ifndef __OTA_H
#define __OTA_H

#define DEBUG_ESP_OTA 1
#define DEBUG_ESP_PORT Serial
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

class OTA
{
public:
    OTA(const char *name);
    ~OTA();
    /**
     * @brief Check for the updates over the air
     * 
     */
    void check();
};

#endif
