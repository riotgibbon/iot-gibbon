#pragma once
#include "sensor.h"
#include "deskmate/ext/sensors/mq2Simple/mq2Simple.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class mq2Sensor
             : public sensor{
                public:
                    mq2Sensor
                    ();
                    mq2Sensor
                    (std::string location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;
                    bool InitSensor();
                    // virtual std::string getType() override;
                private:
                    // Adafruit_Si7021 *sensor;
                    mq2Simple *mq2;
                    // std::string location;
                    std::string getTopic(std::string metric);
                    void readAndSend(std::string metric, int value, deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
            };
        }
    }
}
