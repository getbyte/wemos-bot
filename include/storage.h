#ifndef __STORAGE_H
#define __STORAGE_H

#define MAX_COMMAND_LENGTH 20
#define MAX_USERS_COUNT 5

typedef char ChatId[MAX_COMMAND_LENGTH];
extern ChatId approvedChatId[MAX_USERS_COUNT];

/**
 * @brief Save the settings at SPIFFS
 * 
 */
void saveSettings();
/**
 * @brief Load the settings from the SPIFFS
 * 
 * @param allowRegister allow to register the new user or not
 * @return true settings file exists
 * @return false settings file not found, and it should be created
 */
bool loadSettings(bool allowRegister);

#endif
