#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/veml6075.h"
#include "Arduino.h"


using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {


            bool veml6075::InitSensor(){
                uv = Adafruit_VEML6075();
                  if (! uv.begin()) {
                    Serial.println("Failed to communicate with VEML6075 sensor, check wiring?");
                    return false;
                }
                Serial.println("Found VEML6075 sensor");

                // Set the integration constant
                uv.setIntegrationTime(VEML6075_100MS);
                // Get the integration constant and print it!
                Serial.print("Integration time set to ");
                switch (uv.getIntegrationTime()) {
                    case VEML6075_50MS: Serial.print("50"); break;
                    case VEML6075_100MS: Serial.print("100"); break;
                    case VEML6075_200MS: Serial.print("200"); break;
                    case VEML6075_400MS: Serial.print("400"); break;
                    case VEML6075_800MS: Serial.print("800"); break;
                }
                Serial.println("ms");

                // Set the high dynamic mode
                uv.setHighDynamic(true);
                // Get the mode
                if (uv.getHighDynamic()) {
                    Serial.println("High dynamic reading mode");
                } else {
                    Serial.println("Normal dynamic reading mode");
                }

                // Set the mode
                uv.setForcedMode(false);
                // Get the mode
                if (uv.getForcedMode()) {
                    Serial.println("Forced reading mode");
                } else {
                    Serial.println("Continuous reading mode");
                }

                // Set the calibration coefficients
                uv.setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                                    2.95, 1.74,  // UVB_C and UVB_D coefficients
                                    0.001461, 0.002591); // UVA and UVB responses
                return true;
            }
            veml6075::veml6075()
            {
                _sensorType = "veml6075";
                // hcsr04Sensor("flat");
            }

            veml6075::veml6075(std::string location)
            {
                _location = location;
                _sensorType = "veml6075";

                _isConnected = InitSensor();
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string veml6075::getTopic(std::string metric)
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/").append(metric).append("/").append(_location).append("/").append(_sensorType);
                // Serial.println(topic.c_str());
                return topic;
            }

            void veml6075::readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = floatToString(reading);
                mqtt_buffer_->Publish(message);
            }
            void veml6075::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                 Serial.print("Raw UVA reading:  "); Serial.println(uv.readUVA());
                Serial.print("Raw UVB reading:  "); Serial.println(uv.readUVB());
                Serial.print("UV Index reading: "); Serial.println(uv.readUVI());

                readAndSend("UVA", uv.readUVA(), mqtt_buffer_);
                readAndSend("UVB", uv.readUVB(), mqtt_buffer_);
                readAndSend("uvIndex", uv.readUVI(), mqtt_buffer_);
           
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
