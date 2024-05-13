#ifdef VEML
#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/veml7700.h"
#include "Arduino.h"


using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {


            bool veml7700::InitSensor(){
                veml = Adafruit_VEML7700();
                if (!veml.begin()) {
                    Serial.println("Sensor not found");
                    return false;
                }
                Serial.println("Sensor found");

                veml.setGain(VEML7700_GAIN_1);
                veml.setIntegrationTime(VEML7700_IT_800MS);

                Serial.print(F("Gain: "));
                switch (veml.getGain()) {
                    case VEML7700_GAIN_1: Serial.println("1"); break;
                    case VEML7700_GAIN_2: Serial.println("2"); break;
                    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
                    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
                }

                Serial.print(F("Integration Time (ms): "));
                switch (veml.getIntegrationTime()) {
                    case VEML7700_IT_25MS: Serial.println("25"); break;
                    case VEML7700_IT_50MS: Serial.println("50"); break;
                    case VEML7700_IT_100MS: Serial.println("100"); break;
                    case VEML7700_IT_200MS: Serial.println("200"); break;
                    case VEML7700_IT_400MS: Serial.println("400"); break;
                    case VEML7700_IT_800MS: Serial.println("800"); break;
                }

                //veml.powerSaveEnable(true);
                //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);

                veml.setLowThreshold(10000);
                veml.setHighThreshold(20000);
                veml.interruptEnable(true);
                return true;
            }
            veml7700::veml7700()
            {
                _sensorType = "veml7700";
                // hcsr04Sensor("flat");
            }

            veml7700::veml7700(std::string location)
            {
                _location = location;
                _sensorType = "veml7700";

                _isConnected = InitSensor();
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string veml7700::getTopic(std::string metric)
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/").append(metric).append("/").append(_location).append("/").append(_sensorType);
                // Serial.println(topic.c_str());
                return topic;
            }

            void veml7700::readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = floatToString(reading);
                mqtt_buffer_->Publish(message);
            }
            void veml7700::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                Serial.print("Lux: "); Serial.println(veml.readLux());
                Serial.print("White: "); Serial.println(veml.readWhite());
                Serial.print("Raw ALS: "); Serial.println(veml.readALS());

                readAndSend("Lux", veml.readLux(), mqtt_buffer_);
                readAndSend("White", veml.readWhite(), mqtt_buffer_);
                readAndSend("Raw_ALS", veml.readALS(), mqtt_buffer_);
           
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
#endif