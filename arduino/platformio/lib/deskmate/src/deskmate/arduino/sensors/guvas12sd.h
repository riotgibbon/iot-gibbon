#pragma once

#include "sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {
            class guvas12sd : public sensor
            {
            public:
                guvas12sd();
                guvas12sd(std::string location, int pin);
                void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);

            private:
                // std::string _plant;
                // uint8_t _address[8];
                std::string getTopic(std::string metric);
                void send(std::string metric, std::string value,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
                // std::string getPlantDetails();
                int _pin;

                // OneWire *oneWire;

                // // // Pass our oneWire reference to Dallas Temperature.
                // DallasTemperature _sensors;
            };
        } // namespace sensors
    }     // namespace arduino
} // namespace deskmate
