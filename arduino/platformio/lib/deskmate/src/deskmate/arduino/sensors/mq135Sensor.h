#pragma once
#include "sensor.h"

#include <MQUnifiedsensor.h>

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class mq135Sensor
             : public sensor{
                public:
                    mq135Sensor
                    ();
                    mq135Sensor (std::string location);
                    mq135Sensor (std::string location, int pin);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:

                    MQUnifiedsensor *mq135;
                    int _pin;
                    std::string getTopic(std::string metric);
                   void  readAndSend(std::string metric, float a, float b, deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
            };
        }
    }
}
