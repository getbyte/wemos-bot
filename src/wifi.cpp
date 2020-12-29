#include "wifi.h"
#include "bot.h"

#define maxAttempts 10

const AccessPoint ap[] = {
    {"AP1", "password1"},
    {"AP2", "password2"}
};

const int apCount = sizeof(ap)/sizeof(AccessPoint);

void WifiConnect()
{

  WiFi.mode(WIFI_STA);
  Serial.println("");

  int attempts = 0;
  for (uint8_t apIdx = 0; apIdx < apCount;)
  {
    if (attempts == 0)
    {
      WiFi.begin(ap[apIdx].apName, ap[apIdx].apPassword);
      Serial.printf("Try to connect to %s", ap[apIdx].apName);
    }
    delay(1000);
    Serial.print(" .");
    if (WiFi.status() == WL_CONNECTED)
      break;
    else if (++attempts > maxAttempts)
    {
      delay(500);
      attempts = 0;
      Serial.printf(": %d attempts to connect to %s, connection status is %d\n", maxAttempts, ap[apIdx].apName, (int)WiFi.status());
      WiFi.disconnect(true);
      apIdx++;
      if (apIdx < apCount)
        WiFi.mode(WIFI_STA);
      delay(500);
    }
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    for (;;) {
      Serial.printf("Cannot connect to the provided access points\n");
      digitalWrite(16, HIGH);
      delay(1000);
      digitalWrite(16, LOW);
      delay(1000);
    }
  }
  else
  {
    Serial.printf("\n");
    Serial.printf("Connected to %s\n", WiFi.SSID().c_str());
  }

  secured_client.setCACert((const uint8_t *)TELEGRAM_CERTIFICATE_ROOT, sizeof(TELEGRAM_CERTIFICATE_ROOT)); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}
