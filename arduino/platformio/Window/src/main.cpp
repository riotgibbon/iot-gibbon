#include <Arduino.h>

// #include <queue>
// #include <string>
// #include <vector>


#include "deskmate/app/app.h"

#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/arduino/sensors/soil.h"

using deskmate::app::App;

using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::soil;


void setup() {
  Serial.begin(9600);
  Serial.println("Creating app");
  std::string device  = "mkr1011";  
  std::string location  = "livingroom";
  std::string si7021location  =   ("livingroom/window");

  // si7021 *sensor_si7021 = new si7021(si7021location); 
  // sensor_si7021->InitSensor();

  // soil *bonsai = new soil(location, "bonsai", A0,0);
  


  App app(si7021location,device);
  app.Init();
  app.addSensor(new si7021(si7021location));
  app.addSensor(new soil(location, "bonsai", A0,0));

  Serial.println("Everything setup");
  Serial.print("created ") ;
  // Serial.println( sensor_si7021->getType().c_str()); 
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}

void loop() {}