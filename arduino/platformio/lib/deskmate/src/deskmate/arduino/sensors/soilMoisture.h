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
            class soilMoisture : public sensor
            {
            public:
                soilMoisture();
                soilMoisture(std::string location, std::string plant, int analogPort, int digitalPort);
                void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);

            private:
                std::string _plant;
                // uint8_t _address[8];
                std::string getTopic();
                std::string getPlantDetails();
                int _analogPort ;
                int _digitalPort ;
                // int _plantIndex;

                // OneWire *oneWire;

                // // // Pass our oneWire reference to Dallas Temperature.
                // DallasTemperature _sensors;
            };
        } // namespace sensors
    }     // namespace arduino
} // namespace deskmate
