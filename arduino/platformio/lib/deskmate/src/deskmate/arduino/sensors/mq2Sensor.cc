#include "deskmate/arduino/sensors/mq2Sensor.h"
#include "Arduino.h"
// #include "MQUnifiedsensor.h"
#include "deskmate/ext/sensors/mq2Simple/mq2Simple.h"
#include <string>
#include <sstream>

// #define         Board                   ("AZ-ENVY")
#define Pin (A0) //Analog input 3 of your arduino

// /***********************Software Related Macros************************************/
// #define         Type                    ("MQ-2") //MQ2
// #define         Voltage_Resolution      (5)
// #define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
// #define         RatioMQ2CleanAir        (9.83) //RS / R0 = 9.83 ppm

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {

            char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
            {
                char fmt[20];
                sprintf(fmt, "%%%d.%df", width, prec);
                sprintf(sout, fmt, val);
                return sout;
            }

            bool mq2Sensor::InitSensor()
            {

                mq2 = new mq2Simple(Pin);
                mq2->begin();

                return true;
            }

            mq2Sensor::mq2Sensor()
            {
                _sensorType = "mq2Sensor";
                mq2Sensor("kitchen");
            }

            mq2Sensor::mq2Sensor(std::string location)
            {
                _location = location;
                Serial.print("Initialising mq2Sensor sensor for ");
                Serial.println(_location.c_str());
                _sensorType = "mq2Sensor";
                _isConnected = InitSensor();
            }

            std::string mq2Sensor::getTopic(std::string metric)
            {
                // std::string mode = "test";
                std::string mode = "home";
                std::string topic = mode + "/tele/" + metric + "/" + _location;
                return topic;
            }

            std::string floatToString(float value)
            {
                char buffer[5];
                std::string s = dtostrf(value, 1, 2, buffer);
                return s;
            }

            void  mq2Sensor::readAndSend(std::string metric, int value, deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                 
                    Serial.print(metric.c_str());
                    Serial.print(" : ");
                    Serial.println(value);


                    MQTTMessage message;
                    message.topic = getTopic(metric);;
                    message.payload = std::to_string(value);
                    mqtt_buffer_->Publish(message);
            }

            void mq2Sensor::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                if (_isConnected)
                {
                    Serial.print("reading mq2Sensor: ");
                    Serial.println(_location.c_str());
                    float* values= mq2->read(true);

                    readAndSend("carbonmonoxide", mq2->readCO(),mqtt_buffer_);
                    readAndSend("smoke", mq2->readSmoke(),mqtt_buffer_);
                }
                else
                {
                    Serial.println("not connected");
                }
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
