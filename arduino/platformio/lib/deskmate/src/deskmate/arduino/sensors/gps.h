#pragma once
#include "sensor.h"
#include <Adafruit_GPS.h>


namespace deskmate {
    namespace arduino {
        namespace sensors {


             class gps: public sensor{
                public:
                    gps();
                    gps(std::string location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                private:
                     Adafruit_GPS *GPS;
                     void readAndSend(std::string metric, String reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
           
                    
                    std::string getTopic(std::string metric);

            };
        }
    }
}
