#include <Arduino.h>

#include <queue>
#include <string>
#include <vector>

#include "deskmate/app/app.h"


#include "deskmate/arduino/sensors/mq2Sensor.h"
#include "deskmate/arduino/sensors/SHT3xSensor.h"

using deskmate::app::App;


using deskmate::arduino::sensors::mq2Sensor;
using deskmate::arduino::sensors::sensor;
using deskmate::arduino::sensors::SHT3xSensor;



void setup() {
  Serial.begin(9600);

  std::string device  = "azenvy";  
  std::string location  = "livingroom";


  mq2Sensor *azenvy_mq2 = new mq2Sensor(location); 
  // SHT3xSensor *azenvy_sht30 = new SHT3xSensor(location); 
  

  App app(location,device);
  app.Init();
  // app.setLEDPin(3);
  app.addSensor(azenvy_mq2);
  //  app.addSensor(azenvy_sht30);
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}
void loop() {}

