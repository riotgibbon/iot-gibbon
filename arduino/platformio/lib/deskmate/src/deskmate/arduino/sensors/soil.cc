#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/soil.h"
#include "Arduino.h"
#include "Adafruit_Si7021.h"

#define TRIG_PIN 12
#define ECHO_PIN 13

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {

            struct readings
            {
                std::string temperature;
                std::string humidity;
            };

            // bool hcsr04Sensor::InitSensor(){

            // }
            soil::soil()
            {
                _sensorType = "soil";
                // hcsr04Sensor("flat");
            }
            std::string soil::getPlantDetails()
            {
                std::string details = "Plant: ";
                return details.append(_plant).append(" A").append(std::to_string(_analogPort)).append(" D").append(std::to_string(_digitalPort));
            }
            soil::soil(std::string location, std::string plant, int analogPort, int digitalPort)
            {
                _location = location;
                _sensorType = "soil";
                _plant = plant;
                _analogPort = analogPort;
                _digitalPort = digitalPort;
                pinMode(_digitalPort, OUTPUT); //Set _digitalPort as an OUTPUT
                // InitSensor();
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string soil::getTopic()
            {
                std::string mode = "test";
                std::string topic = mode.append("/tele/soilmoisture/").append(_location).append("/").append(_plant);
                // Serial.println(topic.c_str());
                return topic;
            }

            void soil::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                Serial.print("Reading plant: ");
                Serial.println(getPlantDetails().c_str());

                digitalWrite(_digitalPort, HIGH);  //turn D7 "On"
                (10);                              //wait 10 milliseconds
                int val = analogRead(_analogPort); //Read the SIG value form sensor
                digitalWrite(_digitalPort, LOW);   //turn D7 "Off"
                std::string reading = std::string("Plant: ").append(_plant).append(" : ").append(std::to_string(val));
                Serial.println(reading.c_str());
                Serial.println(getTopic().c_str());

                MQTTMessage message;
                message.topic = getTopic();
                message.payload = std::to_string(val);
                mqtt_buffer_->Publish(message);

                //   HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);
                //  Serial.print("Initialising hcsr04 sensor for ");
                //  Serial.println(location.c_str());
                // Serial.println(hcsr04.ToString());
                // Serial.println("dummy si7021 reading");
                // float temperature = 0;
                // float humidity = 0;
                // temperature=sensor.readTemperature();
                // char tempString[8];
                // dtostrf(temperature, 1, 2, tempString);
                // humidity=sensor.readHumidity();
                // char humString[8];
                // dtostrf(humidity, 1, 2, humString);

                // Serial.print("Humidity:    ");
                // Serial.print(humString);
                // Serial.print("\tTemperature: ");
                // Serial.println(tempString);

                // readings thisReading;
                // thisReading.temperature=tempString;
                // thisReading.humidity=humString;

                // std::string metricTemp = "temperature";
                // std::string metricHumidity = "humidity";

                // MQTTMessage temperatureMessage;
                // temperatureMessage.topic = getTopic(metricTemp);
                // temperatureMessage.payload=thisReading.temperature;

                // MQTTMessage humidityMessage;
                // humidityMessage.topic=getTopic(metricHumidity);
                // humidityMessage.payload =thisReading.humidity;

                // // mqtt_buffer_->Tick();
                // mqtt_buffer_->Publish(temperatureMessage);
                // mqtt_buffer_->Publish(humidityMessage);
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
