#include "deskmate/arduino/sensors/si7021.h"
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

        bool si7021::InitSensor(){
            Serial.println("Si7021 test!");
            
            if (!sensor->begin()) {
                Serial.println("Did not find Si7021 sensor!");
                return false;
            }

            Serial.print("Found model ");
            switch(sensor->getModel()) {
                case SI_Engineering_Samples:
                Serial.print("SI engineering samples"); break;
                case SI_7013:
                Serial.print("Si7013"); break;
                case SI_7020:
                Serial.print("Si7020"); break;
                case SI_7021:
                Serial.print("Si7021"); break;
                case SI_UNKNOWN:
                default:
                Serial.print("Unknown");
            }
            Serial.print(" Rev(");
            Serial.print(sensor->getRevision());
            Serial.print(")");
            Serial.print(" Serial #"); Serial.print(sensor->sernum_a, HEX); Serial.println(sensor->sernum_b, HEX);
            return true;
        }

        si7021::si7021(){
            _sensorType="si7021";
            si7021("kitchen");
        }

        si7021::si7021(std::string location){
            _location=location;
            Serial.print("Initialising Si7101 sensor for ");
            Serial.println(_location.c_str());

            
            _sensorType="si7021";
            sensor = new Adafruit_Si7021();    
            
            _isConnected= InitSensor();
        }

        std::string si7021::getTopic(std::string metric){
            std::string mode = "home";
            std::string topic =mode + "/tele/" + metric +"/" + _location;
            // Serial.println(topic.c_str());
            return topic;
        }
 

        void si7021::readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                Serial.print("Reading : ");
                Serial.print(metric.c_str());
                Serial.print(" : ");
                Serial.println(reading);
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = floatToString(reading);
                mqtt_buffer_->Publish(message);
            }

         void si7021::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_ )  {
                if (_isConnected) {  
                    readAndSend("temperature", sensor->readTemperature(),mqtt_buffer_ );
                    readAndSend("humidity", sensor->readHumidity(),mqtt_buffer_ );  
                }
                else
                {
                    Serial.println("not connected");
                }
            };
        }
    }


}
