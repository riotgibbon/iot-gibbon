#pragma once
#include "deskmate/mqtt/mqtt.h"
#include "Arduino.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {

            class sensor{
                public:
                    // sensor(){};
                    virtual void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer)=0;
                    std::string getLocation(){
                        return _location;
                    }
                    std::string getType(){
                        return _sensorType;
                    };
                protected:
                    std::string _location;
                    std::string _sensorType;
                    bool _isConnected = false;
                    // deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_;
            };


        }
    }
}
