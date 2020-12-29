#include "bot.h"
#include <ESP8266Ping.h>

String unknownCommand[] = {
    "Не понял...",
    "А? Чего???",
    "Выражайтесь яснее, мил человек!",
    "Ничего не понимаю...",
    "Вы уверены, что я могу это сделать?",
    "Чего вы от меня хотите?"
};

void(* softReset) (void) = 0;

const uint32_t REBOOT_DELAY = 5000;

WiFiClientSecure secured_client;

void Bot::sendMessage(const char *msg, const char *destination)
{
    for (int i = 0; i < 5; i++)
    {
        Serial.printf("%d: approvedChatId = %s (len = %d), strcmp = %d, destination = %s (len = %d)\n", i, approvedChatId[i], strlen(approvedChatId[i]), strcmp(destination, approvedChatId[i]), destination, strlen(destination));
        if ((strlen(approvedChatId[i]) == 0) || (strcmp(destination, approvedChatId[i]) && (strlen(destination) > 0)))
            continue;
        Serial.printf("SENDING -> %s!\n", msg);
        bot->sendMessage(approvedChatId[i], msg, "Markdown");
    }
}

void Bot::handleMessages(int numNewMessages)
{
    String answer;
    for (int i = 0; i < numNewMessages; i++)
    {
        int actionPin = -1;
        int chatId = -1;
        int freeId = -1;
        bool needRestart = false;
        telegramMessage &msg = bot->messages[i];

        for (int j = 0; j < 5; j++)
        {
            if (strlen(approvedChatId[j]) == 0)
            {
                freeId = (freeId < 0) ? j : freeId;
                continue;
            }
            if (!strcmp(approvedChatId[j], msg.from_id.c_str()))
            {
                chatId = j;
                break;
            }
        }
        if (chatId < 0)
        {
            if (allowRegister && (freeId >= 0))
            {
                answer = "Добавлен новый пользователь: " + msg.from_name;
                memcpy(&approvedChatId[freeId], msg.from_id.c_str(), msg.from_id.length());
                allowRegister = false;
                saveSettings();
                this->sendMessage(answer.c_str(), (char *)"");
            }
            continue;
        }
        if (msg.text == "/help")
            answer = "Этот бот может перезапускать разные устройства...";
        else if (msg.text == "/cam1")
        {
            actionPin = PIN_CAM1;
            answer = "Перезагрузка камеры №1, подождите... Запрос от " + msg.from_name;
        }
        else if (msg.text == "/cam2")
        {
            actionPin = PIN_CAM2;
            answer = "Перезагрузка камеры №2, подождите... Запрос от " + msg.from_name;
        }
        else if (msg.text == "/modem")
        {
            actionPin = PIN_MODEM;
            answer = "Перезагрузка модема, подождите... Запрос от " + msg.from_name;
        }
        else if (msg.text == "/system")
        {
            needRestart = true;
            answer = "Перезагрузка чатбота, подождите... Запрос от " + msg.from_name;
        }
        else if (msg.text == "/modem-system")
        {
            actionPin = PIN_MODEM;
            needRestart = true;
            answer = "Перезагрузка модема и чатбота, подождите... Запрос от " + msg.from_name;
        }
        else if (msg.text == "/register")
        {
            allowRegister = 1;
            answer = "Следующий подключившийся пользователь будет добавлен в список уведомлений. Запрос от " + msg.from_name;
        }
        else if (msg.text == "/clear")
        {
            allowRegister = 1;
            answer = "Список уведомлений очищен. Запрос от " + msg.from_name;
            SPIFFS.remove("/allow.list");
            memset(&approvedChatId[0], 0, 20 * 5);
            memcpy(&approvedChatId[freeId], msg.from_id.c_str(), msg.from_id.length());
            allowRegister = false;
            saveSettings();
        }
        else
        {
            int idx = random(0, 6);
            answer = unknownCommand[idx];
        }
        String destination = "";
        if (actionPin < 0)
        {
            destination = msg.from_id;
        }
        this->sendMessage(answer.c_str(), (char *)destination.c_str());
        if (actionPin >= 0)
        {
            digitalWrite(actionPin, LOW);
            delay(REBOOT_DELAY);
            digitalWrite(actionPin, HIGH);
        }
        if (needRestart)
        {
            restarttime = millis() + 5000;
            chatbotRestart = true;
        }
    }
}

void Bot::processMessages()
{
    if (millis() - handletime <= messagePullPeriod)
    {
        return;
    }

    int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    while (numNewMessages)
    {
        Serial.println("Processing the messages...");
        this->handleMessages(numNewMessages);
        numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    }
    handletime = millis();
    if (onlineNotify) {
        Serial.println("Try to inform about start");
        sendMessage("Я снова онлайн!", "");
        onlineNotify = false;
    }
    if (chatbotRestart && (millis() - restarttime > 0))
    {
        softReset();
    }
}

void Bot::ping(const char *address, uint32_t period)
{
    if (millis() - pingtime > period)
    {
        bool connectionOk = Ping.ping(address, 4);
        if (connectionOk)
        {
            if (reconnectsCount > 0)
            {
                sprintf(reconnectsNotify, "Я перезагрузил модем %d раз, пока все заработало...", reconnectsCount);
                sendMessage(reconnectsNotify, "");
            }
            reconnectsCount = 0;
        }
        else
        {
            digitalWrite(PIN_MODEM, LOW);
            delay(REBOOT_DELAY);
            digitalWrite(PIN_MODEM, HIGH);
            reconnectsCount++;
        }
        pingtime = millis();
        if (reconnectsCount > 10) 
        {
            softReset();
        }
    }
}

Bot::Bot()
{
    reconnectsCount = 0;
    pingtime = millis();
    handletime = pingtime;
    bot = new UniversalTelegramBot(BOT_TOKEN, secured_client);
    const String commands = F("["
                              "{\"command\":\"help\",  \"description\":\"Подсказка\"},"
                              "{\"command\":\"cam1\", \"description\":\"Перезапуск камеры №1\"},"
                              "{\"command\":\"cam2\", \"description\":\"Перезапуск камеры №2\"},"
                              "{\"command\":\"modem\", \"description\":\"Перезапуск роутера и модема\"},"
                              "{\"command\":\"system\", \"description\":\"Перезапуск чатбота\"},"
                              "{\"command\":\"modem-system\", \"description\":\"Перезапуск роутера, модема и чатбота\"},"
                              "{\"command\":\"register\", \"description\":\"Регистрация нового пользователя\"},"
                              "{\"command\":\"clear\", \"description\":\"Очистка списка\"}"
                              "]");
    bot->setMyCommands(commands);
    onlineNotify = true;
    chatbotRestart = false;
}
