#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/soilTemperature.h"
#include "Arduino.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define TRIG_PIN 12
#define ECHO_PIN 13

#define ONE_WIRE_BUS 5

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {

            soilTemperature::soilTemperature()
            {
                _sensorType = "soilTemperature";
                // hcsr04Sensor("flat");
            }
            // std::string soilTemperature::getPlantDetails()
            // {
            //     std::string details = "Plant: ";
            //     return details.append(_plant); //.append(" A").append(std::to_string(_analogPort)).append(" D").append(std::to_string(_digitalPort));
            // }

            void printAddress(DeviceAddress deviceAddress)
            {
                for (uint8_t i = 0; i < 8; i++)
                {
                    Serial.print("0x");
                    if (deviceAddress[i] < 0x10)
                        Serial.print("0");
                    Serial.print(deviceAddress[i], HEX);
                    if (i < 7)
                        Serial.print(", ");
                }
                Serial.println("");
            }

            soilTemperature::soilTemperature(std::string location)
            {
                _location = location;
                _sensorType = "soilMoisture";
                // _plant = plant;
                // _plantIndex = plantIndex;
                // _sensors = sensors;

                // // _address = deviceAddress;

                // OneWire oneWire(ONE_WIRE_BUS);

                // // // Pass our oneWire reference to Dallas Temperature.
                // // sensors(= new Da (&oneWire);
                // // sensors.begin();
                // _sensors =  DallasTemperature(&oneWire);
                // _sensors.begin();
                // // locate devices on the bus
                // Serial.println("Locating devices...");
                // Serial.print("Found ");
                // int deviceCount = _sensors.getDeviceCount();
                // Serial.print(deviceCount, DEC);
                // Serial.println(" devices.");
                // Serial.println("");

                // Serial.println("Printing addresses...");
                // for (int i = 0; i < deviceCount; i++)
                // {
                //     DeviceAddress Thermometer;
                //     Serial.print("Sensor ");
                //     Serial.print(i);
                //     Serial.print(" : ");
                //     _sensors.getAddress(Thermometer, i);
                //     printAddress(Thermometer);
                // }
            }

            // namespace sensors
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string soilTemperature::getTopic(std::string plant)
            {
                std::string mode = "test";
                std::string topic = mode.append("/tele/temperature/").append(_location).append("/").append(plant);
                // Serial.println(topic.c_str());
                return topic;
            }

            // void getTemperature(DeviceAddress deviceAddress)
            // {
            //     float tempC = _sensors.getTempC(deviceAddress);
            // }


            void soilTemperature::readAndSend(std::string plant, DeviceAddress address, DallasTemperature sensors,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                std::string  topic = getTopic(plant);
              
                Serial.print(topic.c_str());        
                Serial.print(" - ");
                   double tempC = sensors.getTempC(address);
                 String temp_str = String(tempC, 2);
                 Serial.print(temp_str.c_str());
                 Serial.println("C ");


                MQTTMessage message;
                message.topic = topic;
                message.payload = temp_str.c_str();
                mqtt_buffer_->Publish(message);
            }


            void soilTemperature::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                OneWire oneWire(5);
                DallasTemperature sensors(&oneWire);
                sensors.begin();
                sensors.requestTemperatures();

                // std::string plant = "bonsai";
                DeviceAddress bonsaiAddress = {0x28, 0x6D, 0xEB, 0x19, 0x4E, 0x20, 0x01, 0xCC};
                DeviceAddress amaryllisAddress =  {0x28, 0xFE, 0x6D, 0xF9, 0x4D, 0x20, 0x01, 0xDE};
                DeviceAddress arialaAddress =  {0x28, 0x31, 0x2D, 0x07, 0xD6, 0x01, 0x3C, 0x47};
                DeviceAddress yuccaAddress =  {0x28, 0x42, 0x38, 0x07, 0xD6, 0x01, 0x3C, 0x28};

                readAndSend("bonsai", bonsaiAddress, sensors, mqtt_buffer_);
                readAndSend("amaryllis", amaryllisAddress, sensors, mqtt_buffer_);
                readAndSend("ariala", arialaAddress, sensors, mqtt_buffer_);
                readAndSend("yucca", yuccaAddress, sensors, mqtt_buffer_);

                
                // float tempC = sensors.getTempC(address);
                // Serial.print(plant.c_str());        
                // Serial.print(" - ");
                //  Serial.print(tempC);
                //  Serial.println("C ");
                // // address
                // Serial.print(getPlantDetails().c_str());
                // Serial.print(", index: ");
                // Serial.print(_plantIndex);
                // Serial.print(" - ");


                // Serial.println("Locating devices...");
                // Serial.print("Found ");
                // int deviceCount = sensors.getDeviceCount();
                // Serial.print(deviceCount, DEC);
                // Serial.println(" devices.");
                // Serial.println("");

                // Serial.println("Printing addresses...");
                // for (int i = 0; i < deviceCount; i++)
                // {
                //     DeviceAddress Thermometer;
                //     Serial.print("Sensor ");
                //     Serial.print(i);
                //     Serial.print(" : ");
                //     double tempC = sensors.getTempCByIndex(i);
                //     Serial.print(tempC);
                //     Serial.print("C : ");
                //      sensors.getAddress(Thermometer, i);
                //     printAddress(Thermometer);
                // }

                // OneWire oneWire(ONE_WIRE_BUS);

                // Pass our oneWire reference to Dallas Temperature.
                // DallasTemperature sensors(&oneWire);
                // sensors.begin();
                // double tempC = _sensors.getTempCByIndex(_plantIndex);
                // DeviceAddress bonsaiThermometer = {0x28, 0x6D, 0xEB, 0x19, 0x4E, 0x20, 0x01, 0xCC};
                // // double tempC = _sensors.getTempC(bonsaiThermometer);
                // // float tempC = getTemperature(_address);
                // Serial.print(tempC);
                // Serial.print((char)176);
                // Serial.println("C");



                // MQTTMessage message;
                // message.topic = getTopic();
                // message.payload = std::to_string(val);
                // mqtt_buffer_->Publish(message);
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
