# wemos-bot
This bot allows to control devices via Telegram

I'm using one tiny wemos board (ESP8266-based) with this code at the summer house. I installed there two IP cams and 4G-modem, and this allows me to restart the hardware if something goes wrong. For example, a camera may hang on, or modem can lose the connection. 
Bot checks some IP-address from time to time, and restart the router with modem if the ping has been failed. Cams should be restarted manually, but in theory this process also can be automated.
