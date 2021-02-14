#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <queue>
#include <string>
#include <vector>

#include "deskmate/arduino/sensors/sensor.h"
#include "deskmate/arduino/sensors/si7021.h"
#include "deskmate/arduino/sensors/hcsr04Sensor.h"

#include "credentials.h"
#include "deskmate/app/app.h"
// #include "deskmate/arduino/gfx/sharp_mem_display.h"
// #include "deskmate/arduino/input/buttons.h"
// #include "deskmate/arduino/input/crank.h"
#include "deskmate/arduino/net/mqtt.h"
#include "deskmate/arduino/net/wifi.h"
// #include "deskmate/gfx/display.h"  // testing only

#include "deskmate/arduino/sensors/guvas12sd.h"

using deskmate::arduino::sensors::guvas12sd;
using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;
using deskmate::arduino::sensors::hcsr04Sensor;

using deskmate::app::App;
// using deskmate::arduino::gfx::SharpMemDisplay;
// using deskmate::arduino::input::SetupButtonsInterruptHandler;
// // using deskmate::arduino::input::SetupCrankInterruptHandler;
// using deskmate::arduino::net::MQTTManager;
// using deskmate::mqtt::MQTTMessage;
// using deskmate::arduino::net::WiFiManager;


// // Wifi.
// using deskmate::credentials::kWIFIPassword;
// using deskmate::credentials::kWIFISSID;

// // MQTT.
// using deskmate::credentials::kMQTTClientId;
// using deskmate::credentials::kMQTTConfigs;
// using deskmate::credentials::kMQTTFloatingPointSensors;
// using deskmate::credentials::kMQTTPassword;
// using deskmate::credentials::kMQTTPort;
// using deskmate::credentials::kMQTTServer;
// using deskmate::credentials::kMQTTUser;
// using deskmate::credentials::kMQTTWeatherConfigs;


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
 app.addSensor(new guvas12sd(location, A0));
  while (true) {
    // wifi_manager.MaybeReconnect();
    app.Tick();
  }
}

void loop() {}