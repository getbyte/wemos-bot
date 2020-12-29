#ifndef __WIFI_H
#define __WIFI_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

typedef struct AccessPoint
{
  const char *apName;
  const char *apPassword;
} AccessPoint;

/**
 * @brief Establish connection to one of the AccessPoints provided in the ap array (see wifi.cpp)
 * 
 */
void WifiConnect();

#endif
