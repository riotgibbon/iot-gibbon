#include "deskmate/arduino/sensors/mq9Sensor.h"
#include "Arduino.h"
// #include "MQUnifiedsensor.h"
// #include "deskmate/ext/sensors/mq9Simple/mq9Simple.h"
#include <MQUnifiedsensor.h>
#include <string>
#include <sstream>

#define         Board                   ("ESP8266")
// #define         Pin                     (A0)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-9") //mq9
#define         Voltage_Resolution     (5)// (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         Ratiomq9CleanAir        (9.83) //RS / R0 = 9.83 ppm 

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {



            bool mq9Sensor::InitSensor()
            {

            /*
                    Exponential regression:
                GAS     | a      | b
                LPG     | 1000.5 | -2.186
                CH4     | 4269.6 | -2.648
                CO      | 599.65 | -2.244
                */
                mq9 = new MQUnifiedsensor(Board, Voltage_Resolution, ADC_Bit_Resolution, _pin, Type);
                mq9->setA(599.65 ); mq9->setB(-2.244);
                mq9->init(); 

                Serial.print("Calibrating please wait.");
                float calcR0 = 0;
                for(int i = 1; i<=10; i ++)
                {
                    mq9->update(); // Update data, the arduino will be read the voltage on the analog pin
                    calcR0 += mq9->calibrate(Ratiomq9CleanAir);
                    Serial.print(".");
                }
                mq9->setR0(calcR0/10);
                Serial.println("  done!.");
                
                if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
                if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
 

                return true;
            }

            mq9Sensor::mq9Sensor()
            {
                _sensorType = "mq9Sensor";
                mq9Sensor("kitchen");
            }

            mq9Sensor::mq9Sensor(std::string location)
            {
                mq9Sensor(location, A0);
            }

             mq9Sensor::mq9Sensor(std::string location, int pin)
            {   
                _pin = pin;
                _location = location;
                Serial.print("Initialising mq9Sensor sensor for ");
                Serial.println(_location.c_str());
                _sensorType = "mq9Sensor";
                _isConnected = InitSensor();
            }

            std::string mq9Sensor::getTopic(std::string metric)
            {
                // std::string mode = "test";
                std::string mode = "home";
                std::string topic = mode + "/tele/" + metric + "/" + _location + "/mq9";
                return topic;
            }



            void  mq9Sensor::readAndSend(std::string metric, std::string  value, deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                 
                    Serial.print(metric.c_str());
                    Serial.print(" : ");
                    Serial.println(value.c_str());


                    MQTTMessage message;
                    message.topic = getTopic(metric);;
                    message.payload = (value);
                    mqtt_buffer_->Publish(message);
            }

            void mq9Sensor::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                if (_isConnected)
                {
                    Serial.print("reading mq9Sensor: ");
                    Serial.println(_location.c_str());
                 
                    mq9->update();
                    float CO = mq9->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
                    std::string coStr = floatToString( CO);
                    readAndSend("carbonmonoxide", coStr, mqtt_buffer_);
                    
  

                }
                else
                {
                    Serial.println("not connected");
                }
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
