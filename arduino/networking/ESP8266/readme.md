# ESP8266 ESP-01S

Wifi module for Arduino

https://www.amazon.co.uk/gp/product/B01LK83TX0



need to install board
https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/



https://create.arduino.cc/projecthub/Niv_the_anonymous/esp8266-beginner-tutorial-project-6414c8


https://www.instructables.com/Getting-Started-With-Esp-8266-Esp-01-With-Arduino-/


Flashing procedure:
* reset Arduino (press reset when powering on)
* put pin2 (green) to GND
* remove power to esp
* upload sketch
* add power when trying to connect

https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html

```
#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(74880);
  Serial.println();

  WiFi.begin("VM9729091", "bnb3cwmvNhXh");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
```

```
Connecting.......
Connected, IP address: 192.168.0.68
```

IP address:  192.168.0.68



http://yaab-arduino.blogspot.com/p/wifiesp.html


WiFiEsp.h
https://github.com/bportaluri/WiFiEsp

https://learn.sparkfun.com/tutorials/esp8266-wifi-shield-hookup-guide#installing-the-esp8266-at-library

https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library
