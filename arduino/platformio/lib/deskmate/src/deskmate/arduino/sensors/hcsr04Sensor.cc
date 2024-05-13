#ifdef HC_SR04

#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/hcsr04Sensor.h"
#include "Arduino.h"



#define TRIG_PIN 12
#define ECHO_PIN 13


using deskmate::mqtt::MQTTMessage;


namespace deskmate {
    namespace arduino {
        namespace sensors {

          struct readings
            {
                std::string temperature;
                std::string humidity;
            };    
        
        // bool hcsr04Sensor::InitSensor(){
            
        // }
        hcsr04Sensor::hcsr04Sensor(){
            _sensorType="hcsr04";
            hcsr04Sensor("flat");
        }
        hcsr04Sensor::hcsr04Sensor(std::string location){
            _location=location;
            _sensorType="hcsr04";
            // InitSensor();
        }
        std::string hcsr04Sensor::getTopic(std::string metric){
            std::string mode = "home";
            std::string topic =mode + "/tele/" + metric +"/" + _location;
            Serial.println(topic.c_str());
            return topic;
        }
  
         void hcsr04Sensor::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_ )  {
              HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);    
             Serial.print("Initialising hcsr04 sensor for ");
             Serial.println(_location.c_str());
            Serial.println(hcsr04.ToString());
  
                };
            }
        }


    }
#endif