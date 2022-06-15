# E-Ink WiFi Weather Widget

This project is intednded to run on ESP8266 e-paper driver board NodeMCU module connected to 2.7" e-paper display and optional DS18B20 temperature sensor.
Weather forecast data is being downloaded from openweathermap.org JSON API. It is super energy efficient. You should be able to power it from a single 3V battery for a long time.

![ppelikan_e-ink_wifi_weather_widget](https://user-images.githubusercontent.com/6893111/173921241-fa8fbf55-b73e-4740-8884-73fc6e82b5b2.jpeg)

## Features
- Live forecast data updated every 15min
- Nice look and compact size
- Energy efficient design
- Multilanguage support
- Easy to modify and upgrade

## Needed libraries
- ESP8266 Arduino https://github.com/esp8266/Arduino
- Adafruit GFX Library https://github.com/adafruit/Adafruit-GFX-Library
- GxEPD https://github.com/ZinggJM/GxEPD
- U8g2 for Adafruit GFX https://github.com/ppelikan/U8g2_for_Adafruit_GFX (make sure you use mine version which contains RAM usage fixes for ESP8266)
- ArduinoJson 6.x https://github.com/bblanchon/ArduinoJson
- OneWire https://github.com/PaulStoffregen/OneWire
- Arduino Temperature Control Library https://github.com/milesburton/Arduino-Temperature-Control-Library

## Hardware modifications
- In order to make deep sleep state possible, the GPIO16 (D0 on PCB) pin has to be shorted to the RST pin. After this modification you will need to press "Flash" button every time you upload code into the board and after that reset the power by disconnecting the board for a moment. 
- If you want to see the in-door measured temperature, connect the One Wire data line of the DS18B20 temperature sensor to the GPIO2 (D2 on PCB) along with the 4k7 pullup resistor.

## License
