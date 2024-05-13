#ifdef VEML
#pragma once
#include "Arduino.h"
#include "sensor.h"
#include "Adafruit_VEML6075.h"

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {
            class veml6075 : public sensor
            {
            public:
                veml6075();
                veml6075(std::string location);
                void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);

            private:
                bool InitSensor();
                Adafruit_VEML6075 uv; 
                std::string getTopic(std::string metric);
                void readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
            };
        } // namespace sensors
    }     // namespace arduino
} // namespace deskmate
#endif