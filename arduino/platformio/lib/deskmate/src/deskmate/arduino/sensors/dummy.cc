#include "deskmate/arduino/sensors/sensor.h"
#include "Arduino.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {

         void dummy::read() {
                    Serial.println("dummy reading");
        
                };
            }
        }
    }
