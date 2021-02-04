#include <Arduino.h>

#include <queue>
#include <string>
#include <vector>

#include "deskmate/app/app.h"


#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/arduino/sensors/mq9Sensor.h"

using deskmate::app::App;


using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;
using deskmate::arduino::sensors::mq9Sensor;



void setup() {
  Serial.begin(9600);

  std::string device  = "esp32";  
  std::string location  = "kitchen";


   si7021 *sensor_si7021 = new si7021(location);  
   mq9Sensor *mq9 = new mq9Sensor(location); 

  App app(location,device);
  app.Init();
  app.addSensor(sensor_si7021);
  app.addSensor(mq9);
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}
void loop() {}

