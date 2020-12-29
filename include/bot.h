#ifndef __BOT_H
#define __BOT_H

#include "storage.h"
#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

extern WiFiClientSecure secured_client;

#define PIN_CAM1 5
#define PIN_CAM2 4
#define PIN_MODEM 16

class Bot
{
public:
    Bot();
    ~Bot();
    /**
     * @brief Method where bot should process all the incoming messages
     */
    void processMessages();
    /**
     * @brief Send the message to the defined user_id
     * 
     * @param msg Text of the message
     * @param destination user_id which should get this message. If user_id length is 0, the message will be delivered to each known user
     * @return true if message has been delivered successfully
     * @return false if some went wrong during the sending
     */
    void sendMessage(const char *msg, const char *destination);
    /**
     * @brief Check if it's time to ping some IP-address and ping it if needed
     * 
     * @param address IP-addres which should be checked
     * @param period how often the bot should generate pings to the IP-address
     */
    void ping(const char* address, uint32_t period);

private:
    const uint64_t messagePullPeriod = 1000;
    char reconnectsNotify[120];
    bool allowRegister;
    bool onlineNotify;
    bool chatbotRestart;
    uint32_t reconnectsCount;
    uint64_t handletime;
    uint64_t pingtime;
    uint64_t restarttime;
    UniversalTelegramBot *bot;
    void handleMessages(int numNewMessages);
};

#endif
