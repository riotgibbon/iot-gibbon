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
                    bool isConnected(){
                        return _isConnected;
                    }
                protected:
                    std::string _location;
                    std::string _sensorType;
                    bool _isConnected = false;
                    // deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_;
                    char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
                    {
                        char fmt[5];
                        sprintf(fmt, "%%%d.%df", width, prec);
                        sprintf(sout, fmt, val);
                        return sout;
                    }

                    std::string floatToString(float value)
                    {
                        char buffer[5];
                        std::string s = dtostrf(value, 1, 2, buffer);
                        return s;
                    }

                    
            };


        }
    }
}
