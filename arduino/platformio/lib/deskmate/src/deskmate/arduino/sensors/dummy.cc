#include "deskmate/arduino/sensors/dummy.h"
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
