#pragma once
#include "sensor.h"
#include "Adafruit_PM25AQI.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class pm25: public sensor{
                public:
                    pm25();
                    pm25(std::string location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    void broadcast(deskmate::app::Broadcast *broadcast) override;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:
                    Adafruit_PM25AQI *sensor;
                     void readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
           
                    
                    std::string getTopic(std::string metric);

            };
        }
    }
}
