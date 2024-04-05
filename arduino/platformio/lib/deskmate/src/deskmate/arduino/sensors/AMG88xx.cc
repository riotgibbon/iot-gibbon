#include "deskmate/arduino/sensors/AMG88xx.h"
#include "Arduino.h"
#include "Adafruit_AMG88xx.h"
#include <string>  
#include <sstream>
#include <ArduinoJson.h>

using deskmate::mqtt::MQTTMessage;


namespace deskmate {
    namespace arduino {
        namespace sensors {

        // struct readings
        //     {
        //         std::string temperature;
        //         std::string humidity;
        //     };    
        // char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
        //     char fmt[20];
        //     sprintf(fmt, "%%%d.%df", width, prec);
        //     sprintf(sout, fmt, val);
        //     return sout;
        // }

        bool AMG88xx::InitSensor(){
            bool status = sensor->begin();
            if (!status) {
                Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
                while (1);
            }
                return true;
            }
 
        AMG88xx::AMG88xx(){
            _sensorType="AMG88xx";
            // si7021("kitchen");
        }

        AMG88xx::AMG88xx(std::string location){
            _location=location;
            Serial.print("Initialising AMG88xx sensor for ");
            Serial.println(_location.c_str());

            
            _sensorType="AMG88xx";
            sensor = new Adafruit_AMG88xx();    
            
            _isConnected= InitSensor();
        }

        std::string AMG88xx::getTopic(std::string metric){
            std::string mode = "home";
            std::string topic =mode + "/tele/" + metric +"/" + _location;
            // Serial.println(topic.c_str());
            return topic;
        }
 

        void AMG88xx::readAndSend(std::string metric, String reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                Serial.print("Reading : ");
                Serial.print(metric.c_str());
                Serial.print(" : ");
                // Serial.println(reading);
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = reading.c_str();
                mqtt_buffer_->Publish(message);
            }

         void AMG88xx::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_ )  {
                if (_isConnected) {  
                    float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
                    sensor->readPixels(pixels);
                    const size_t CAPACITY = JSON_ARRAY_SIZE(64);
                    StaticJsonDocument<CAPACITY> doc;
                    JsonArray array = doc.to<JsonArray>();

                    
                    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
                        array.add(pixels[i-1]);
                    }

                    // String jsonString = JSON.stringify(myArray); 
                    // Serial.print("JSON.stringify(myArray) = ");
                    // Serial.println(jsonString);
                    
                    // doc["ir"] = pixels;
                    String output;
                    serializeJson(doc, output);

                    readAndSend("irgrideye", output,mqtt_buffer_ );
                    
                }
                else
                {
                    Serial.println("not connected");
                }
            };
        }
    }


}
