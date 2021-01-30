#pragma once
#include "sensor.h"
#include "deskmate/ext/sensors/SHT3x/SHT3x.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class SHT3xSensor: public sensor{
                public:
                    SHT3xSensor();
                    SHT3xSensor(std::string location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:
                    SHT3x *sensor;
                
                    // std::string location;
                    std::string getTopic(std::string metric);
            };
        }
    }
}
