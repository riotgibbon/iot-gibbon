#include "sensor.h"
#include "Adafruit_Si7021.h"

namespace deskmate {
    namespace arduino {
        namespace sensors {


             class si7021: public sensor{
                public:
                    si7021();
                    si7021(String _location);
                    void read() override;
                private:
                    Adafruit_Si7021 sensor;
                    bool InitSensor();
                    String location;
            };
        }
    }
}