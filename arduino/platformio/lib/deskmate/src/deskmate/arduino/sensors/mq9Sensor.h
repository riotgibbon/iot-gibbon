#pragma once
#include "sensor.h"
#include "deskmate/ext/sensors/mq2Simple/mq2Simple.h"
#include <MQUnifiedsensor.h>

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class mq9Sensor
             : public sensor{
                public:
                    mq9Sensor
                    ();
                    mq9Sensor (std::string location);
                     mq9Sensor (std::string location, int pin);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:

                    MQUnifiedsensor *mq9;
                    int _pin;
                    std::string getTopic(std::string metric);
                    void readAndSend(std::string metric, std::string value, deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
            };
        }
    }
}
