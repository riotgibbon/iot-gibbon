#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/hcsr04Sensor.h"
#include "Arduino.h"
#include "Adafruit_Si7021.h"


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
            sensorType="hcsr04";
            hcsr04Sensor("flat");
        }
        hcsr04Sensor::hcsr04Sensor(std::string _location){
            location=_location;
            sensorType="hcsr04";
            // InitSensor();
        }
        std::string hcsr04Sensor::getTopic(std::string metric){
            std::string mode = "home";
            std::string topic =mode + "/tele/" + metric +"/" + location;
            Serial.println(topic.c_str());
            return topic;
        }
  
         void hcsr04Sensor::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_ )  {
              HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);    
             Serial.print("Initialising hcsr04 sensor for ");
             Serial.println(location.c_str());
            Serial.println(hcsr04.ToString());
            // Serial.println("dummy si7021 reading");
            // float temperature = 0;
            // float humidity = 0;
            // temperature=sensor.readTemperature();
            // char tempString[8];
            // dtostrf(temperature, 1, 2, tempString);    
            // humidity=sensor.readHumidity();
            // char humString[8];
            // dtostrf(humidity, 1, 2, humString);

            // Serial.print("Humidity:    ");
            // Serial.print(humString);
            // Serial.print("\tTemperature: ");
            // Serial.println(tempString);

            // readings thisReading;
            // thisReading.temperature=tempString;
            // thisReading.humidity=humString;
            
            // std::string metricTemp = "temperature";
            // std::string metricHumidity = "humidity";

            // MQTTMessage temperatureMessage;
            // temperatureMessage.topic = getTopic(metricTemp);
            // temperatureMessage.payload=thisReading.temperature;

            // MQTTMessage humidityMessage;
            // humidityMessage.topic=getTopic(metricHumidity);
            // humidityMessage.payload =thisReading.humidity;  
            
            
            // // mqtt_buffer_->Tick();
            // mqtt_buffer_->Publish(temperatureMessage);
            // mqtt_buffer_->Publish(humidityMessage);
        
                };
            }
        }


    }
