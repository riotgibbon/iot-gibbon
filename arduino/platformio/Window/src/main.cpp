#include <Arduino.h>

// #include <queue>
// #include <string>
// #include <vector>

#include <DallasTemperature.h>

#include "deskmate/app/app.h"

#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/arduino/sensors/soilMoisture.h"
#include "deskmate/arduino/sensors/soilTemperature.h"
#include "deskmate/arduino/sensors/guvas12sd.h"
#include "deskmate/arduino/sensors/mq9Sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

using deskmate::app::App;

using deskmate::arduino::sensors::guvas12sd;
using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::soilMoisture;
using deskmate::arduino::sensors::soilTemperature;
using deskmate::arduino::sensors::mq9Sensor;

void setup()
{
  Serial.begin(9600);
  Serial.println("Creating app");
  std::string device = "mkr1011";
  std::string location = "livingroom";
  std::string windowLocation = ("livingroom/window");


  App app(windowLocation, device);
  app.Init();

  app.addSensor(new si7021(windowLocation));
  app.addSensor(new soilMoisture(location, "bonsai", A0, 0));
  app.addSensor(new soilMoisture(location, "amaryllis", A1, 1));
  app.addSensor(new soilMoisture(location, "aralia", A2, 2));
  app.addSensor(new soilMoisture(location, "yucca", A3, 3));
  app.addSensor(new soilTemperature( location));
  app.addSensor(new mq9Sensor(location,A4));

  app.addSensor(new guvas12sd(windowLocation, A6));

  Serial.println("Everything setup");

  Serial.print("created ");
  // Serial.println( sensor_si7021->getType().c_str());
  while (true)
  {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}

void loop() {}