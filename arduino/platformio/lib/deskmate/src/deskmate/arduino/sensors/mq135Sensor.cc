#include "deskmate/arduino/sensors/mq135Sensor.h"
#include "Arduino.h"
#include <MQUnifiedsensor.h>
#include <string>
#include <sstream>

#define         Board                   ("ESP8266")
// #define         Pin                     (A0)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-135") //mq135
#define         Voltage_Resolution     (5)// (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {



            bool mq135Sensor::InitSensor()
            {

            /*
                    Exponential regression:
                GAS     | a      | b
                LPG     | 1000.5 | -2.186
                CH4     | 4269.6 | -2.648
                CO      | 599.65 | -2.244
                */
                mq135 = new MQUnifiedsensor(Board, Voltage_Resolution, ADC_Bit_Resolution, _pin, Type);
                
                mq135->init(); 

                  Serial.print("Calibrating please wait.");
                    float calcR0 = 0;
                    for(int i = 1; i<=10; i ++)
                    {
                        mq135->update(); // Update data, the arduino will be read the voltage on the analog pin
                        calcR0 += mq135->calibrate(RatioMQ135CleanAir);
                        Serial.print(".");
                    }
                    mq135->setR0(calcR0/10);
                    Serial.println("  done!.");
                    
                    if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); return false;}
                    if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); return false;}

                return true;
            }

            mq135Sensor::mq135Sensor()
            {
                _sensorType = "mq135Sensor";
                mq135Sensor("kitchen");
            }

            mq135Sensor::mq135Sensor(std::string location)
            {
                mq135Sensor(location, A0);
            }

             mq135Sensor::mq135Sensor(std::string location, int pin)
            {   
                _pin = pin;
                _location = location;
                Serial.print("Initialising mq135Sensor sensor for ");
                Serial.println(_location.c_str());
                _sensorType = "mq135Sensor";
                _isConnected = InitSensor();

            }

            std::string mq135Sensor::getTopic(std::string metric)
            {
                // std::string mode = "test";
                std::string mode = "home";
                std::string topic = mode + "/tele/" + metric + "/" + _location + "/mq135";
                return topic;
            }



           void  mq135Sensor::readAndSend(std::string metric, float a, float b, deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {


                    mq135->setA(a); mq135->setB(b); // Configurate the ecuation values to get CH4 concentration
                    float value = mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    

                    Serial.print(metric.c_str());
                    Serial.print(" : ");
                    Serial.println(value);


                    MQTTMessage message;
                    message.topic = getTopic(metric);;
                    message.payload = floatToString(value);
                    mqtt_buffer_->Publish(message);
            }

            void mq135Sensor::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                if (_isConnected)
                {
                    Serial.print("reading mq135Sensor: ");
                    Serial.println(_location.c_str());
    
                    /*
                        Exponential regression:
                    GAS      | a      | b
                    CO       | 605.18 | -3.937  
                    Alcohol  | 77.255 | -3.18 
                    CO2      | 110.47 | -2.862
                    Tolueno  | 44.947 | -3.445
                    NH4      | 102.2  | -2.473
                    Acetona  | 34.668 | -3.369
                    */

                    mq135->update();


                    // mq2->setA(574.25); mq2->setB(-2.222); // Configurate the ecuation values to get CH4 concentration
                    // float LPG = mq2->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    readAndSend("carbonmonoxide", 605.18 ,-3.937 ,mqtt_buffer_);
                    readAndSend("alcohol", 77.255 ,-3.18,mqtt_buffer_);
                    readAndSend("ammonium", 102.2  ,-2.473,mqtt_buffer_);

                }
                else
                {
                    Serial.println("not connected");
                }
            };

        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
