# Window sensor

## Board

initially az-delivery-devkit-v4 

[env:az-delivery-devkit-v4]
platform = espressif32
board = az-delivery-devkit-v4



## Sensors

### Si7021
sparkfun/SparkFun Si7021 Humidity and Temperature Sensor @ ^1.0.3

3.3/5v
GND
SDA - 11 - blue
SCL - 12 - yellow


### Soil

A0 - D0 - bonsai 
A1 - D1 - amaryllis
A2 - D2 - aralia
A3 - D3 - yucca 

### SoilTemperature

get addresses
https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/


bonsai      0x28, 0x6D, 0xEB, 0x19, 0x4E, 0x20, 0x01, 0xCC
amaryllis   0x28, 0xFE, 0x6D, 0xF9, 0x4D, 0x20, 0x01, 0xDE
ariala      0x28, 0x31, 0x2D, 0x07, 0xD6, 0x01, 0x3C, 0x47
yucca -     0x28, 0x42, 0x38, 0x07, 0xD6, 0x01, 0x3C, 0x28


Printing addresses...
Sensor 0 : 0x28, 0x42, 0x38, 0x07, 0xD6, 0x01, 0x3C, 0x28 - yucca
Sensor 1 : 0x28, 0xFE, 0x6D, 0xF9, 0x4D, 0x20, 0x01, 0xDE - amaryllis
Sensor 2 : 0x28, 0x31, 0x2D, 0x07, 0xD6, 0x01, 0x3C, 0x47 - ariala
Sensor 3 : 0x28, 0x6D, 0xEB, 0x19, 0x4E, 0x20, 0x01, 0xCC - bonsai



## MQTT test
mosquitto_sub -h 192.168.0.63 -t test


## Checking available ports

ls /dev/cu*

LDNML35SLVDR:Window toby.evans$ ls /dev/cu*
/dev/cu.AirbyAfterShokz-CSRGAIA-2       /dev/cu.AirbyAfterShokz-CSRGAIA-7       /dev/cu.usbmodem144101
/dev/cu.AirbyAfterShokz-CSRGAIA-3       /dev/cu.Bluetooth-Incoming-Port