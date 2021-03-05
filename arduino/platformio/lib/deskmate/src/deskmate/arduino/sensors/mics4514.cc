#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/mics4514.h"
#include "Arduino.h"

#include<string>

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {


            bool mics4514::InitSensor(){
                int PRE_HEAT_SECONDS = 10;
                  pinMode(_prePin, OUTPUT);
  

                Serial.println("MiCS-4514 Test Read");
                Serial.print("Preheating...");

                // Wait for preheating
                digitalWrite(_prePin, 1);
                delay(PRE_HEAT_SECONDS * 1000);
                digitalWrite(_prePin, 0);
                Serial.println("Done");
                return true;
            }
            mics4514::mics4514()
            {
                _sensorType = "mics4514";
                // hcsr04Sensor("flat");
            }

            mics4514::mics4514(std::string location, int prePin, int noxPin, int redPin)
            {
                _location = location;
                _prePin= prePin;
                _noxPin = noxPin;
                _redPin =redPin;

                _sensorType = "mics4514";

                _isConnected = InitSensor();
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string mics4514::getTopic(std::string metric)
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/").append(metric).append("/").append(_location).append("/").append(_sensorType);
                // Serial.println(topic.c_str());
                return topic;
            }

            void mics4514::readAndSend(std::string metric, std::string  reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = (reading);
                mqtt_buffer_->Publish(message);
            }
            void mics4514::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                int vnox_value = analogRead(_noxPin);
                int vred_value = analogRead(_redPin);

                //  String(sensorValue, 2).c_str()
                 Serial.print("Raw vnox:  "); Serial.println(vnox_value);
                Serial.print("Raw vred:  "); Serial.println(vred_value);

                // Serial.print("UV Index reading: "); Serial.println(uv.readUVI());

                readAndSend("vnox",  std::to_string(vnox_value), mqtt_buffer_);
                readAndSend("vred", std::to_string(vred_value), mqtt_buffer_);
                // readAndSend("UVB", uv.readUVB(), mqtt_buffer_);
                // readAndSend("uvIndex", uv.readUVI(), mqtt_buffer_);
           
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
