#pragma once
#include "sensor.h"
#include "Adafruit_Si7021.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class si7021: public sensor{
                public:
                    si7021();
                    si7021(std::string location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:
                    Adafruit_Si7021 *sensor;
                    
                    // std::string location;
                    std::string getTopic(std::string metric);
            };
        }
    }
}
