#include "storage.h"
#include <Arduino.h>
#include <FS.h>

ChatId approvedChatId[5];

void saveSettings()
{
  File fsUploadFile = SPIFFS.open("/allow.list", "w");
  if (fsUploadFile)
  {
    fsUploadFile.write((uint8_t *)approvedChatId, 20 * 5);
    fsUploadFile.close();
  }
}

bool loadSettings(bool allowRegister)
{
  if (!SPIFFS.exists("/allow.list"))
  {
    return false;
  }
  File fsLoadFile = SPIFFS.open("/allow.list", "r");
  if (fsLoadFile)
  {
    fsLoadFile.read((uint8_t *)approvedChatId, 20 * 5);
    fsLoadFile.close();
    for (int i = 0; i < 5; i++)
    {
      // FIXME this code seems to be wrong, should be fixed
      if (strlen(approvedChatId[i]) > 0)
        allowRegister = false;
      Serial.println(approvedChatId[i]);
    }
  }
  return true;
}
