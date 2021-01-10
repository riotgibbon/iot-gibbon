#ifndef DESKMATE_ARDUINO_SENSORS_SI7021_H
#define DESKMATE_ARDUINO_SENSORS_SI7021_H

#include "sensor.h"
#include "Adafruit_Si7021.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class si7021: public sensor{
                public:
                    si7021();
                    si7021(std::string _location);
                    void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) override;
                 
                private:
                    Adafruit_Si7021 sensor;
                    bool InitSensor();
                    std::string location;
                    std::string getTopic(std::string metric);
            };
        }
    }
}
#endif 