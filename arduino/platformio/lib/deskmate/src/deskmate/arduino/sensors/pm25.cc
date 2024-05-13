#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/pm25.h"
#include "Arduino.h"

#include<string>

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {


            bool pm25::InitSensor(){

                if (!sensor->begin_I2C()) {      // connect to the sensor over I2C
                    Serial.println("Could not find PM 2.5 sensor!");
                    return false;
                }

                Serial.println("Connected PM 2.5 sensor!");
                return true;

            }
            pm25::pm25()
            {
                _sensorType = "pm25";
     
            }

            pm25::pm25(std::string location)
            {
                _location = location;


                _sensorType = "pm25";
                sensor = new Adafruit_PM25AQI();    

                _isConnected = InitSensor();
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string pm25::getTopic(std::string metric)
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/").append(metric).append("/").append(_location);
                // Serial.println(topic.c_str());
                return topic;
            }

            void pm25::readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = floatToString(reading);
                mqtt_buffer_->Publish(message);
            }
            void pm25::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                Serial.print("Starting PM25 read");
                PM25_AQI_Data data =PM25_AQI_Data() ;
                sensor->read(&data);

                Serial.println("AQI reading success");

                Serial.println();
                Serial.println(F("---------------------------------------"));
                Serial.println(F("Concentration Units (standard)"));
                Serial.println(F("---------------------------------------"));
                Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
                Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
                Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
                Serial.println(F("Concentration Units (environmental)"));
                Serial.println(F("---------------------------------------"));
                Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
                Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
                Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
                Serial.println(F("---------------------------------------"));
                Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
                Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
                Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
                Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
                Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
                Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
                Serial.println(F("---------------------------------------"));
                
      
                readAndSend("pm10", (data.pm100_env), mqtt_buffer_);
                readAndSend("pm2.5", (data.pm25_env), mqtt_buffer_);
                readAndSend("pm1", (data.pm10_env), mqtt_buffer_);

            };
            void pm25::broadcast(deskmate::app::Broadcast *broadcast) 
            {

                PM25_AQI_Data data =PM25_AQI_Data() ;
                sensor->read(&data);

                broadcast->sendMessage("pm10", floatToString(data.pm100_env));
                broadcast->sendMessage("pm2.5", floatToString(data.pm25_env));
                broadcast->sendMessage("pm1",floatToString (data.pm10_env));
            }
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
