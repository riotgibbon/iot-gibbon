#include "deskmate/arduino/sensors/mq2Sensor.h"
#include "Arduino.h"
// #include "MQUnifiedsensor.h"
// #include "deskmate/ext/sensors/mq2Simple/mq2Simple.h"
#include <MQUnifiedsensor.h>
#include <string>
#include <sstream>

#define         Board                   ("ESP8266")
#define         Pin                     (A0)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-2") //MQ2
#define         Voltage_Resolution     (3.3)// (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ2CleanAir        (9.83) //RS / R0 = 9.83 ppm 

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

                // mq2 = new mq2Simple(Pin);                
                // mq2->begin();

                mq2 = new MQUnifiedsensor(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
                mq2->setA(36974); mq2->setB(-3.109);
                mq2->init(); 

                Serial.print("Calibrating please wait.");
                float calcR0 = 0;
                for(int i = 1; i<=10; i ++)
                {
                    mq2->update(); // Update data, the arduino will be read the voltage on the analog pin
                    calcR0 += mq2->calibrate(RatioMQ2CleanAir);
                    Serial.print(".");
                }
                mq2->setR0(calcR0/10);
                Serial.println("  done!.");
                
                if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
                if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
 

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
                    // float* values= mq2->read(true);

                    // readAndSend("carbonmonoxide", mq2->readCO(),mqtt_buffer_);
                    // readAndSend("smoke", mq2->readSmoke(),mqtt_buffer_);
                    
                      /*
                        Exponential regression:
                        Gas    | a      | b
                        H2     | 987.99 | -2.162
                        LPG    | 574.25 | -2.222
                        CO     | 36974  | -3.109
                        Alcohol| 3616.1 | -2.675
                        Propane| 658.71 | -2.168
                    */

                    mq2->update();
                    // mq2->setA(574.25); mq2->setB(-2.222); // Configurate the ecuation values to get CH4 concentration
                    // float LPG = mq2->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    

                    // mq2->setA(36974); mq2->setB(-3.109); // Configurate the ecuation values to get CH4 concentration
                    float CO = mq2->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    mq2->serialDebug();// Will print the table on the serial port
                    
                    // mq2->setA(3616.1 ); mq2->setB(-2.675); // Configurate the ecuation values to get CH4 concentration
                    // float Alcohol = mq2->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    
                    // mq2->setA(30000000); mq2->setB(-8.308); // Configurate the ecuation values to get CH4 concentration
                    // float Smoke = mq2->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    
                    // Serial.print("|    "); Serial.print(LPG);
                    // Serial.print("    |    "); Serial.print(CH4);
                    Serial.print("    |    "); Serial.print(CO);
                    // Serial.print("    |    "); Serial.print(Alcohol);
                    // Serial.print("    |    "); Serial.print(Smoke);
                    Serial.println("    |");

                }
                else
                {
                    Serial.println("not connected");
                }
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
