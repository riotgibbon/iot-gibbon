#ifndef DESKMATE_APP_APP_H
#define DESKMATE_APP_APP_H
#include <Arduino.h>
#include <iostream>
#include <memory>
#include <vector>

#include "deskmate/app/config.h"
// #include "deskmate/gfx/display.h"
// #include "deskmate/gfx/screens/window.h"
#include "deskmate/input/input.h"
#include "deskmate/mqtt/mqtt.h"
#include "deskmate/arduino/sensors/sensor.h"
// #include "deskmate/arduino/sensors/si7021.h"
// #include "deskmate/arduino/sensors/hcsr04Sensor.h"
#include "deskmate/arduino/net/mqtt.h"
#include "deskmate/arduino/net/wifi.h"
#include "credentials.h"

#include "deskmate/app/broadcast.h"




namespace deskmate {
namespace app {
namespace {

using deskmate::app::MQTTConfig;
using deskmate::app::Broadcast;
using deskmate::arduino::sensors::sensor;
using deskmate::arduino::net::MQTTManager;
using deskmate::arduino::net::WiFiManager;

// using deskmate::arduino::sensors::si7021;
// using deskmate::arduino::sensors::hcsr04Sensor;
// using deskmate::gfx::Display;
}  // namespace


class App {
 public:
  // App( deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer)
  //     :  mqtt_buffer_(mqtt_buffer) {}


  App(  std::string location, std::string device ){
    _location=location;
    _device= device;
  }

  bool Init();
  bool InitWiFi(std::string client);
  bool InitMQTT();
  bool InitBroadcast(std::string client);
  void addSensor(sensor* newSensor);
  void setLEDPin(int ledPin);
   
  bool Tick();  
  
  // readings GetReadings();


 private:
 void setDateTime();
 deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_;
  
 Broadcast *broadcast_;

 WiFiManager *wifi_manager_;
  // void dummyReading(); 
  // MQTTManager mqtt_buffer_;
  // sensor thisSensor;
  std::string _location;
  std::string _device;
  int _ledPin=2;
};

}  // namespace app
}  // namespace deskmate

#endif  // DESKMATE_APP_APP_H