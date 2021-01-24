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





## MQTT test
mosquitto_sub -h 192.168.0.63 -t test