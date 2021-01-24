#include "deskmate/arduino/sensors/mq2.h"
#include "Arduino.h"
#include "Adafruit_Si7021.h"
#include <string>  
#include <sstream>

using deskmate::mqtt::MQTTMessage;


namespace deskmate {
    namespace arduino {
        namespace sensors {

        struct readings
            {
                std::string temperature;
                std::string humidity;
            };    
        char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
            char fmt[20];
            sprintf(fmt, "%%%d.%df", width, prec);
            sprintf(sout, fmt, val);
            return sout;
        }

        bool mq2::InitSensor(){
            // Serial.println("Si7021 test!");
            
            // if (!sensor->begin()) {
            //     Serial.println("Did not find Si7021 sensor!");
            //     return false;
            // }

            // Serial.print("Found model ");
            // switch(sensor->getModel()) {
            //     case SI_Engineering_Samples:
            //     Serial.print("SI engineering samples"); break;
            //     case SI_7013:
            //     Serial.print("Si7013"); break;
            //     case SI_7020:
            //     Serial.print("Si7020"); break;
            //     case SI_7021:
            //     Serial.print("Si7021"); break;
            //     case SI_UNKNOWN:
            //     default:
            //     Serial.print("Unknown");
            // }
            // Serial.print(" Rev(");
            // Serial.print(sensor->getRevision());
            // Serial.print(")");
            // Serial.print(" Serial #"); Serial.print(sensor->sernum_a, HEX); Serial.println(sensor->sernum_b, HEX);
            return true;
        }

        mq2::mq2(){
            _sensorType="mq2";
            mq2("kitchen");
        }

        mq2::mq2(std::string location){
            _location=location; 
            Serial.print("Initialising mq2 sensor for ");
            Serial.println(_location.c_str());

            
            _sensorType="mq2";
            // sensor = new Adafruit_Si7021();    
            
            // _isConnected= InitSensor();
        }

        std::string mq2::getTopic(std::string metric){
            std::string mode = "test";
            std::string topic =mode + "/tele/" + metric +"/" + _location;
            // Serial.println(topic.c_str());
            return topic;
        }
 
        std::string floatToString(float value){
            char buffer[5];
            std::string s = dtostrf(value, 1, 2, buffer);
            return s;
        }

         void mq2::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_ )  {
                if (_isConnected) {  
                    Serial.print("reading mq2: ");
                    Serial.println(_location.c_str());
                    //need to do this for some reason, otherwise results won't display
                    // char buffer[5];
                //     String t = dtostrf(0.0, 1, 4, buffer);

                //     std::string metricTemp = "temperature";
                //     std::string metricHumidity = "humidity";

                //     MQTTMessage temperatureMessage;
                //     temperatureMessage.topic = getTopic(metricTemp);
                //     temperatureMessage.payload = floatToString(sensor->readTemperature());

                //     MQTTMessage humidityMessage;
                //     humidityMessage.topic=getTopic(metricHumidity);
                //     humidityMessage.payload =floatToString(sensor->readHumidity());  
                    

                //     mqtt_buffer_->Publish(temperatureMessage);
                //     mqtt_buffer_->Publish(humidityMessage);
                }
                else
                {
                    Serial.println("not connected");
                }
            };
        }
    }


}
