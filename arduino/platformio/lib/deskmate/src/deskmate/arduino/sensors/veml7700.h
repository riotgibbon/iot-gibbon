#pragma once
#include "Arduino.h"
#include "sensor.h"
#include <Adafruit_VEML7700.h>

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {
            class veml7700 : public sensor
            {
            public:
                veml7700();
                veml7700(std::string location);
                void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);

            private:
                bool InitSensor();
                Adafruit_VEML7700 veml; 
                std::string getTopic(std::string metric);
                void readAndSend(std::string metric, float reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
            };
        } // namespace sensors
    }     // namespace arduino
} // namespace deskmate
