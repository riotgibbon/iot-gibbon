#pragma once
#include "Arduino.h"
#include "sensor.h"
// #include "Adafruit_mics4514.h"

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {
            class mics4514 : public sensor
            {
            public:
                mics4514();
                mics4514(std::string location, int prePin, int noxPin, int redPin);
                void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);

            private:
                bool InitSensor();
                int _prePin;
                int _noxPin;
                int _redPin;
                // Adafruit_mics4514 uv; 
                std::string getTopic(std::string metric);
                void readAndSend(std::string metric, std::string  reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
            };
        } // namespace sensors
    }     // namespace arduino
} // namespace deskmate
