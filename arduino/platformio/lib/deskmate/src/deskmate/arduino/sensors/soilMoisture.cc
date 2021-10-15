#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/soilMoisture.h"
#include "Arduino.h"


#define TRIG_PIN 12
#define ECHO_PIN 13

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {

            struct readings
            {
                std::string temperature;
                std::string humidity;
            };

            // bool hcsr04Sensor::InitSensor(){

            // }
            soilMoisture::soilMoisture()
            {
                _sensorType = "soilMoisture";
                // hcsr04Sensor("flat");
            }
            std::string soilMoisture::getPlantDetails()
            {
                std::string details = "Plant: ";
                return details.append(_plant).append(" A").append(std::to_string(_analogPort)).append(" D").append(std::to_string(_digitalPort));
            }
            soilMoisture::soilMoisture(std::string location, std::string plant, int analogPort, int digitalPort)
            {
                _location = location;
                _sensorType = "soilMoisture";
                _plant = plant;
                _analogPort = analogPort;
                _digitalPort = digitalPort;
                pinMode(_digitalPort, OUTPUT); //Set _digitalPort as an OUTPUT
                _isConnected = true;
                // InitSensor();
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string soilMoisture::getTopic()
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/soilmoisture/").append(_location).append("/").append(_plant);
                // Serial.println(topic.c_str());
                return topic;
            }

            void soilMoisture::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                Serial.print("Reading plant: ");
                Serial.println(getPlantDetails().c_str());

                digitalWrite(_digitalPort, HIGH);  //turn D7 "On"
                (10);                              //wait 10 milliseconds
                int val = analogRead(_analogPort); //Read the SIG value form sensor
                digitalWrite(_digitalPort, LOW);   //turn D7 "Off"
                std::string reading = std::string("Plant: ").append(_plant).append(" : ").append(std::to_string(val));
                Serial.println(reading.c_str());
                Serial.println(getTopic().c_str());

                MQTTMessage message;
                message.topic = getTopic();
                message.payload = std::to_string(val);
                mqtt_buffer_->Publish(message);

              
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
