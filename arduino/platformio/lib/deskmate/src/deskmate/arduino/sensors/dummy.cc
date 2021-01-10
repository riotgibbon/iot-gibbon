#include "deskmate/arduino/sensors/dummy.h"
#include "Arduino.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {

         void dummy::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) {
                    Serial.println("dummy reading");
        
                };
            }
        }
    }
