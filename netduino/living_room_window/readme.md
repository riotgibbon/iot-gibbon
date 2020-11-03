# Netduino Living Room Window monitoring

recording soil moisture at living room window with NetduinoPlus, running MicroFramework 4.1

Limitations - MF4.1 very memory constrained, unwilling to upgrade in case it all goes wrong

## Sensor Inputs

### Soil Moisture

Using [SparkFun moisture sensors](https://www.sparkfun.com/products/13322)

Moisture code reading based on [this](https://www.electromaker.io/project/view/monitor-a-plants-soil-moisture-with-netduino-and-xamarin) project. Connect a digital and anlog pin, power the digital pin before reading, then switch off - otherwise putting power into the wet soil will corrode the reader.

Readings theoretically from 0 (dry) to 1023 (saturated). Run the readings over time and calibrate with an [external reader](https://www.amazon.co.uk/gp/product/B07H83JLSM) to gauge true range, and map back as a percentage rating. 

### Temperature/moisture from SI7021 

Understand the impact of proximity of window and radiator on soil moisture levels.

Core code taken from http://netduino.foundation/Library/Sensors/Atmospheric/SI7021/

SI7021 is I2C device;
http://blog.codeblack.nl/post/NetDuino-Getting-Started-with-I2C.aspx

* sda - blue - A4
* scl - yellow - A5

Returns explicit temperature and humidity readings

### UV

Understand the amount of light available in the location with [Analog UV Light Sensor Breakout - GUVA-S12SD](https://www.adafruit.com/product/1918) from [RS] (https://uk.rs-online.com/web/p/sensor-development-tools/1245472)

Supply a voltage and read from the analog port


## Outputs

### IFTTT

Create a csv style string, and send to IFTTT maker endpoint, `living_room_window` metric as a single delimited value. Short-term, http requests seem to cause memory issues in MF4.1

### MQTT

Preferred option, [this code](https://github.com/danielan/NetduinoMQTT) still very functional

topic structure: `home/{metricType}/{metric}/{location}/{device}`

sample topics and messages:
```
home/tele/soilmoisture/livingroom/aralia 839
home/tele/soilmoisture/livingroom/bonsai 826
home/tele/soilmoisture/livingroom/amaryllis 571
home/tele/temperature/livingroom/window 20.37
home/tele/humidity/livingroom/window 69.66
home/tele/humidityAdjusted/livingroom/window 34.83
home/tele/lightReading/livingroom/window 0
home/tele/lightVoltage/livingroom/window 0
home/tele/uvIndex/livingroom/window 0
```