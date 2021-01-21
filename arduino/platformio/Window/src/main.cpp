#include <Arduino.h>
// #include <PubSubClient.h>


// #include <queue>
// #include <string>
// #include <vector>

// #include "credentials.h"
#include "deskmate/app/app.h"
// #include "deskmate/arduino/gfx/sharp_mem_display.h"
// #include "deskmate/arduino/input/buttons.h"
// #include "deskmate/arduino/input/crank.h"
// #include "deskmate/arduino/net/mqtt.h"
// #include "deskmate/arduino/net/wifi.h"
// #include "deskmate/gfx/display.h"  // testing only

#include "deskmate/arduino/sensors/si7021.h"


using deskmate::app::App;
// using deskmate::arduino::gfx::SharpMemDisplay;
// using deskmate::arduino::input::SetupButtonsInterruptHandler;
// using deskmate::arduino::input::SetupCrankInterruptHandler;

// Display.
// using deskmate::credentials::kCSPin;
// using deskmate::credentials::kDisplayHeight;
// using deskmate::credentials::kDisplayWidth;
// using deskmate::credentials::kMOSIPin;
// using deskmate::credentials::kSCKPin;

// using deskmate::arduino::net::MQTTManager;
// using deskmate::mqtt::MQTTMessage;
// using deskmate::arduino::net::WiFiManager;

// Wifi.
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

using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;


void setup() {
  Serial.begin(9600);
  Serial.println("Creating app");
  std::string device  = "mkr1011";  
  std::string location  = "livingroom/window";


  si7021 *sensor_si7021 = new si7021(location); 
  sensor_si7021->InitSensor();


  App app(location,device);
  app.Init();
  app.addSensor(sensor_si7021);
  Serial.println("Everything setup");
  Serial.print("created ") ;
  Serial.println( sensor_si7021->getType().c_str()); 
  while (true) {
    //wifi_manager.MaybeReconnect();
    app.Tick();
  }
}

void loop() {}