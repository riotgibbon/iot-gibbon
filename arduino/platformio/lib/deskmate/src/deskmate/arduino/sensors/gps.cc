#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/GPS.h"
#include "Arduino.h"
#include <ArduinoJson.h>

#include<string>
#define GPSECHO true

using deskmate::mqtt::MQTTMessage;

namespace deskmate
{
    namespace arduino
    {
        namespace sensors
        {
            uint32_t timer = millis();

            bool gps::InitSensor(){

                // Serial.begin(115200);
                Serial.println("Adafruit I2C GPS library basic test!");
        
                // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
                GPS->begin(0x10);  // The I2C address to use is 0x10
                // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
                GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
                 // uncomment this line to turn on only the "minimum recommended" data
                //GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
                // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
                // the parser doesn't care about other sentences at this time
                // Set the update rate
                GPS->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
                // For the parsing code to work nicely and have time to sort thru the data, and
                // print it out we don't suggest using anything higher than 1 Hz

                // Request updates on antenna status, comment out to keep quiet
                GPS->sendCommand(PGCMD_ANTENNA);

                delay(1000);

                // Ask for firmware version
                GPS->println(PMTK_Q_RELEASE);
                return true;

            }
            gps::gps()
            {
                _sensorType = "gps";
     
            }

            gps::gps(std::string location)
            {
                _location = location;
                _sensorType = "gps";
                GPS = new Adafruit_GPS(&Wire);
                _isConnected = InitSensor();
                Serial.println(_isConnected);
            }
            //"home/tele/soilmoisture/livingroom/yucca";
            std::string gps::getTopic(std::string metric)
            {
                std::string mode = "home";
                std::string topic = mode.append("/tele/").append(metric).append("/").append(_location);
                // Serial.println(topic.c_str());
                return topic;
            }

            void gps::readAndSend(std::string metric, String reading,  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_){
                
                
                MQTTMessage message;
                message.topic = getTopic(metric);
                message.payload = reading.c_str();
                mqtt_buffer_->Publish(message);
            }
            void gps::read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_)
            {
                Serial.print("Starting gps read");
                while (true) {    
                char c = GPS->read();
                // if you want to debug, this is a good time to do it!
                if (GPSECHO)
                    if (c) Serial.print(c);
                // if a sentence is received, we can check the checksum, parse it...
                if (GPS->newNMEAreceived()) {
                    // a tricky thing here is if we print the NMEA sentence, or data
                    // we end up not listening and catching other sentences!
                    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
                    Serial.println(GPS->lastNMEA()); // this also sets the newNMEAreceived() flag to false
                    if (!GPS->parse(GPS->lastNMEA())) // this also sets the newNMEAreceived() flag to false
                    return; // we can fail to parse a sentence in which case we should just wait for another
                }

                // approximately every 2 seconds or so, print out the current stats
                if (millis() - timer > 5000) {
                    timer = millis(); // reset the timer
                    Serial.print("\nTime: ");
                    if (GPS->hour < 10) { Serial.print('0'); }
                    Serial.print(GPS->hour, DEC); Serial.print(':');
                    if (GPS->minute < 10) { Serial.print('0'); }
                    Serial.print(GPS->minute, DEC); Serial.print(':');
                    if (GPS->seconds < 10) { Serial.print('0'); }
                    Serial.print(GPS->seconds, DEC); Serial.print('.');
                    if (GPS->milliseconds < 10) {
                    Serial.print("00");
                    } else if (GPS->milliseconds > 9 && GPS->milliseconds < 100) {
                    Serial.print("0");
                    }
                    Serial.println(GPS->milliseconds);
                    Serial.print("Date: ");
                    Serial.print(GPS->day, DEC); Serial.print('/');
                    Serial.print(GPS->month, DEC); Serial.print("/20");
                    Serial.println(GPS->year, DEC);
                    Serial.print("Fix: "); Serial.print((int)GPS->fix);
                    Serial.print(" quality: "); Serial.println((int)GPS->fixquality);
                    if (GPS->fix) {
                        Serial.print("Location: ");
                        Serial.print(GPS->latitude, 4); Serial.print(GPS->lat);
                        Serial.print(", ");
                        Serial.print(GPS->longitude, 4); Serial.println(GPS->lon);
                        Serial.print("Speed (knots): "); Serial.println(GPS->speed);
                        Serial.print("Angle: "); Serial.println(GPS->angle);
                        Serial.print("Altitude: "); Serial.println(GPS->altitude);
                        Serial.print("Satellites: "); Serial.println((int)GPS->satellites);



                        JsonDocument doc;
            
                        doc["latitude"] =GPS->latitudeDegrees;
                        doc["longitude"] =GPS->longitudeDegrees;
                        doc["speed"] =GPS->speed;
                        doc["angle"] =GPS->angle;
                        doc["altitude"] =GPS->altitude;
                        doc["satellites"] =GPS->satellites;
                        String output;
                        serializeJson(doc, output);

                        readAndSend("location", output,mqtt_buffer_ );
                    }
                    return;
                }
      
                // readAndSend("pm10", (data.pm100_env), mqtt_buffer_);
                // readAndSend("pm2.5", (data.pm25_env), mqtt_buffer_);
                // readAndSend("pm1", (data.pm10_env), mqtt_buffer_);
                
            };
        } // namespace sensors
    }     // namespace arduino

} // namespace deskmate
}