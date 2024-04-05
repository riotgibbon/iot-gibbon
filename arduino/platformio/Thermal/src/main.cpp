#include <Arduino.h>

#include <queue>
#include <string>
#include <vector>

#include "deskmate/app/app.h"
#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/AMG88xx.h"


using deskmate::arduino::sensors::AMG88xx;


using deskmate::app::App;
// using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;
// using deskmate::arduino::sensors::mq135Sensor;



void setup() {
  Serial.begin(9600);

  std::string device  = "ESP32";  
  std::string location  = "kitchen/thermal";

  App app(location,device);
  app.Init();
  app.addSensor(new AMG88xx(location));
  // app.addSensor(new mq135Sensor(location, A0 ));
  // app.addSensor(new veml7700(location));
  while (true) {
    app.Tick();
  }
}
void loop() {}

