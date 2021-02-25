#include <Arduino.h>

#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/app/app.h"
#include "deskmate/arduino/sensors/mq135Sensor.h"

#include "deskmate/arduino/sensors/veml7700.h"
using deskmate::arduino::sensors::veml7700;


using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;
using deskmate::arduino::sensors::mq135Sensor;

using deskmate::app::App;

void setup() {
  Serial.begin(9600);

  std::string device  = "esp32";  
  std::string location  = "bedroom";


  // si7021 *sensor_si7021 = new si7021(location); 
// mq135Sensor *sensormq135Sensor= new mq135Sensor(location,A0);
  App app(location,device);
  app.Init();
  app.addSensor(new si7021(location));
  // app.addSensor(new mq135Sensor(location));
  app.addSensor(new mq135Sensor(location,A0));
    app.addSensor(new veml7700(location));
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}
void loop() {}