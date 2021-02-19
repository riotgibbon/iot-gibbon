#include <Arduino.h>
// #include <PubSubClient.h>
// #include <WiFi.h>

#include <queue>
#include <string>
#include <vector>

#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/arduino/sensors/hcsr04Sensor.h"


#include "deskmate/app/app.h"

#include "deskmate/arduino/sensors/guvas12sd.h"

#include "deskmate/arduino/sensors/veml6075.h"
using deskmate::arduino::sensors::veml6075;
using deskmate::arduino::sensors::guvas12sd;
using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;
using deskmate::arduino::sensors::hcsr04Sensor;
using deskmate::arduino::sensors::veml6075;

using deskmate::app::App;


void setup() {
  Serial.begin(9600);

  std::string device  = "esp32";

  std::string location  = "test";

  // std::string client = 
  // si7021 sensor_si7021 = si7021("bedroom");
  // hcsr04Sensor *myHcsr04 = new hcsr04Sensor(location);
  
  App app( location, device);
  app.Init();
  // app.addSensor(myHcsr04);
 app.addSensor(new veml6075(location));
  while (true) {
    // wifi_manager.MaybeReconnect();
    app.Tick();
  }
}

void loop() {}