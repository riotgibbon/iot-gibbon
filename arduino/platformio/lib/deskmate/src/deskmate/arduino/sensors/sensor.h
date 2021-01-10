#ifndef DESKMATE_ARDUINO_SENSORS_SENSOR_H
#define DESKMATE_ARDUINO_SENSORS_SENSOR_H

#include "deskmate/mqtt/mqtt.h"


namespace deskmate {
    namespace arduino {
        namespace sensors {

            class sensor{
                public:
                    sensor(){};
                    virtual void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);
                    
                private:
                    // std::string location;
                    // deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_;
            };


        }
    }
}
#endif 