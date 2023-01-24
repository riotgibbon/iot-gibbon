#include <Arduino.h>

#include "deskmate/arduino/sensors/sensor.h"
// #include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/app/app.h"
// #include "deskmate/arduino/sensors/mq135Sensor.h"
#include "deskmate/arduino/sensors/veml7700.h"
#include "deskmate/arduino/sensors/pm25.h"

// mics4514
using deskmate::arduino::sensors::sensor;
#include "deskmate/arduino/sensors/mics4514.h"
using deskmate::arduino::sensors::mics4514;
using deskmate::arduino::sensors::veml7700;
using deskmate::arduino::sensors::pm25;

// using deskmate::arduino::sensors::si7021;

// using deskmate::arduino::sensors::mq135Sensor;

using deskmate::app::App;

void setup() {
  Serial.begin(9600);

  std::string device  = "ESP8266";  
  std::string location  = "livingroom/desk";


  // si7021 *sensor_si7021 = new si7021(location); 
// mq135Sensor *sensormq135Sensor= new mq135Sensor(location,A0);
  App app(location,device);
  app.Init();
  // app.setLEDPin(LED_BUILTIN);



  // app.addSensor(new mics4514(location, 8, A0, A7));
  app.addSensor(new pm25(location));
  app.addSensor(new veml7700(location));
  
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}
void loop() {}