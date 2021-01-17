#pragma once
#include "deskmate/mqtt/mqtt.h"
#include "Arduino.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {

            class sensor{
                public:
                    sensor(){};
                    virtual void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer)=0;
                    std::string getLocation(){
                        return location;
                    }
                    std::string getType(){
                        return sensorType;
                    };
                protected:
                    std::string location;
                    std::string sensorType;
                    // deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_;
            };


        }
    }
}
