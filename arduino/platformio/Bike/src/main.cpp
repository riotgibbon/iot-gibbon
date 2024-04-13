 #include <Arduino.h>

#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/app/app.h"
#include "deskmate/arduino/sensors/mq135Sensor.h"
// #include "deskmate/arduino/sensors/veml7700.h"
#include "deskmate/arduino/sensors/pm25.h"
#include "deskmate/arduino/sensors/gps.h"

// mics4514
using deskmate::arduino::sensors::sensor;
// #include "deskmate/arduino/sensors/mics4514.h"
// using deskmate::arduino::sensors::mics4514;
// using deskmate::arduino::sensors::veml7700;
using deskmate::arduino::sensors::pm25;
using deskmate::arduino::sensors::gps;
using deskmate::arduino::sensors::si7021;

using deskmate::arduino::sensors::mq135Sensor;

using deskmate::app::App;

std::string device  = "ESP8266";  
std::string location  = "bike";
App app(location,device);

void setup() {
  Serial.begin(9600);

  
  if (app.Init()){

    app.addSensor(new pm25(location));
    app.addSensor(new gps(location));
    app.addSensor(new si7021(location));
    app.addSensor(new mq135Sensor(location,A0));
    while (true) {
      //wifi_manager.MaybeReconnect();
      app.Tick();
    }
  }
}
void loop() {
  // app.Tick();
}