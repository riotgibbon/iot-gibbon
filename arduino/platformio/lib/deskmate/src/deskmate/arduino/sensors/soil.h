#pragma once

#include "sensor.h"
#include "hcsr04.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {
             class soil: public sensor{
                public:
                    soil();
                    soil(std::string location, std::string plant, int analogPort, int digitalPort);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    
                private:
                    // HCSR04 hcsr04;
                    // bool InitSensor();
                    // std::string location;
                    std::string _plant;
                    int _analogPort;
                    int _digitalPort;
                    std::string getTopic();
                    std::string getPlantDetails();
            };
        }
    }
}
