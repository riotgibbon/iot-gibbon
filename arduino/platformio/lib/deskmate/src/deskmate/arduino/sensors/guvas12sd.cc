#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/guvas12sd.h"
#include "Arduino.h"
#include <string>


using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {

            guvas12sd::guvas12sd()
            {
                _sensorType = "uv";
                // hcsr04Sensor("flat");
            }
 

            guvas12sd::guvas12sd(std::string location, int pin)
            {
                _location = location;
                _sensorType = "uv";
                _pin=pin;  
            }


            //home/tele/lightReading/livingroom/window
            //home/tele/lightVoltage/livingroom/window
            //home/tele/uvIndex/livingroom/window

            std::string guvas12sd::getTopic(std::string metric)
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/").append(metric).append("/").append(_location);
                // Serial.println(topic.c_str());
                return topic;
            }

     

            void guvas12sd::send(std::string metric, std::string value,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                std::string  topic = getTopic(metric);
                MQTTMessage message;
                message.topic = topic;
                message.payload = value.c_str();
                mqtt_buffer_->Publish(message);
            }


            void guvas12sd::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {

                // http://arduinolearning.com/code/arduino-guva-s12sd-uv-sensor.php



                    float sensorVoltage; 
                    float sensorValue;
                    float voltage = 5.0;
                    sensorValue = analogRead(_pin);
                    sensorVoltage = sensorValue/1024*voltage;
                    int uvIndex = (int)(sensorVoltage / 0.1);
                    Serial.print("sensor pin = ");
                    Serial.println(_pin);
                    Serial.print("sensor reading = ");
                    Serial.print(sensorValue);
                    Serial.println("");
                    Serial.print("sensor voltage = ");
                    Serial.print(sensorVoltage);
                    Serial.println(" V");
                    Serial.print("uv Index = ");
                    Serial.println(uvIndex);

                    send("lightReading",   String(sensorValue, 2).c_str(), mqtt_buffer_);
                    send("lightVoltage",   String(sensorVoltage, 2).c_str(), mqtt_buffer_);
                    send("uvIndex",   std::to_string(uvIndex), mqtt_buffer_);

                // MQTTMessage message;
                // message.topic = getTopic();
                // message.payload = std::to_string(val);
                // mqtt_buffer_->Publish(message);
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
