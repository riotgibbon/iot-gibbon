#include <Arduino.h>

#include <queue>
#include <string>
#include <vector>

#include "deskmate/app/app.h"


#include "deskmate/arduino/sensors/mq2Sensor.h"


using deskmate::app::App;


using deskmate::arduino::sensors::mq2Sensor;
using deskmate::arduino::sensors::sensor;




void setup() {
  Serial.begin(9600);

  std::string device  = "azenvy";  
  std::string location  = "kitchen";


  mq2Sensor *azenvy_mq2 = new mq2Sensor(location); 

  App app(location,device);
  app.Init();
  app.addSensor(azenvy_mq2);
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}
void loop() {}

