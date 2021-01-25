#include <Arduino.h>

// #include <queue>
// #include <string>
// #include <vector>

#include <DallasTemperature.h>

#include "deskmate/app/app.h"

#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/arduino/sensors/soilMoisture.h"
#include "deskmate/arduino/sensors/soilTemperature.h"

#include <OneWire.h>
#include <DallasTemperature.h>

using deskmate::app::App;

using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::soilMoisture;
using deskmate::arduino::sensors::soilTemperature;

void setup()
{
  Serial.begin(9600);
  Serial.println("Creating app");
  std::string device = "mkr1011";
  std::string location = "livingroom";
  std::string si7021location = ("livingroom/window");

  // si7021 *sensor_si7021 = new si7021(si7021location);
  // sensor_si7021->InitSensor();

  // soil *bonsai = new soil(location, "bonsai", A0,0);

  // OneWire *oneWire = new OneWire(5);

  // Pass our oneWire reference to Dallas Temperature.

  // DallasTemperature *sensors = new DallasTemperature(oneWire);
  // sensors->begin();
  // delay(1000);
  DeviceAddress bonsaiThermometer = {0x28, 0x6D, 0xEB, 0x19, 0x4E, 0x20, 0x01, 0xCC};

  App app(si7021location, device);
  app.Init();

  // app.addSensor(new si7021(si7021location));
  app.addSensor(new soilMoisture(location, "bonsai", A0,0));

  // app.addSensor(new soilTemperature(sensors, location, "yucca", 0));
  // app.addSensor(new soilTemperature(sensors, location, "amaryllis",1));
  // app.addSensor(new soilTemperature(location, "ariala",2));
  app.addSensor(new soilTemperature( location));

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