#ifdef DALLAS
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
            class soilTemperature : public sensor
            {
            public:
                soilTemperature();
                soilTemperature(std::string location);
                void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);

            private:
                // std::string _plant;
                // uint8_t _address[8];
                std::string getTopic(std::string plant);
                void readAndSend(std::string plant, DeviceAddress address, DallasTemperature sensors,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_);
                // std::string getPlantDetails();
                int _plantIndex;

                // OneWire *oneWire;

                // // // Pass our oneWire reference to Dallas Temperature.
                DallasTemperature _sensors;
            };
        } // namespace sensors
    }     // namespace arduino
} // namespace deskmate
#endif