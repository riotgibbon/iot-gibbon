#ifdef AMG88XX
#pragma once
#include "sensor.h"
#include "Adafruit_AMG88xx.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class AMG88xx: public sensor{
                public:
                    AMG88xx();
                    AMG88xx(std::string location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:
                    Adafruit_AMG88xx *sensor;
                     void readAndSend(std::string metric, String reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
           
                    
                    std::string getTopic(std::string metric);
            };
        }
    }
}
#endif