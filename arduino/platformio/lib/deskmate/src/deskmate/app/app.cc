
#include "deskmate/app/app.h"
#include <Arduino.h>
#include <vector>
#include <iterator>

// #include "deskmate/arduino/sensors/dummy.h"
// #include "deskmate/arduino/sensors/sensor.h"

#include "deskmate/app/config.h"
// #include "deskmate/gfx/components/mqtt_circle_horizontal_list_item.h"
// #include "deskmate/gfx/components/mqtt_list_item.h"
// #include "deskmate/gfx/components/mqtt_vertical_bar_horizontal_list_item.h"
// #include "deskmate/gfx/components/text_list_item.h"
// #include "deskmate/gfx/display.h"
// #include "deskmate/gfx/screens/horizontal_list.h"
// #include "deskmate/gfx/screens/list.h"
// #include "deskmate/gfx/screens/window.h"
#include "deskmate/input/input.h"
#include "deskmate/mqtt/mqtt.h"

// #include "Adafruit_Si7021.h"

namespace deskmate {
namespace app {
namespace {
using deskmate::app::MQTTConfig;
using deskmate::app::MQTTFloatingPointSensorConfig;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;

using deskmate::arduino::net::MQTTManager;
using deskmate::arduino::net::WiFiManager;


// Wifi.
using deskmate::credentials::kWIFIPassword;
using deskmate::credentials::kWIFISSID;

// MQTT.
using deskmate::credentials::kMQTTClientId;
using deskmate::credentials::kMQTTConfigs;
using deskmate::credentials::kMQTTFloatingPointSensors;
using deskmate::credentials::kMQTTPassword;
using deskmate::credentials::kMQTTPort;
using deskmate::credentials::kMQTTServer;
using deskmate::credentials::kMQTTUser;
using deskmate::credentials::kMQTTWeatherConfigs;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds

std::vector<sensor* > sensors;


}  // namespace

 

 

bool App::Init() {

    // InitSensor();
    // thisSensor = si7021("bedroom");
    startMillis = millis();  //initial start time
  std::string delim  = "-";
  std::string client = _device.append(delim).append(_location);

  Serial.print("Client: ");
  Serial.println(client.c_str());

  Serial.print("Connecting to : ");
  Serial.println(kWIFISSID);
  WiFiManager *wifi_manager= new WiFiManager(kWIFISSID, kWIFIPassword);
  MQTTManager *mqtt_manager = new MQTTManager(kMQTTServer, kMQTTPort, kMQTTUser, kMQTTPassword, client.c_str());
  if (!wifi_manager->Connect()) {
    Serial.println("Unable to connect to WiFi.");
    return false;
  }
  Serial.print("Connected to WiFi: ");
  Serial.println(kWIFISSID);
  if (!mqtt_manager->Connect()) {
    Serial.println("Unable to connect to the MQTT server.");
    return false;
  }

  mqtt_buffer_= mqtt_manager;
  wifi_manager_= wifi_manager;

  Serial.print("Connected to the MQTT server ");
  Serial.println(kMQTTServer);
   
  Serial.println("trying test message");
  MQTTMessage msg;
  msg.topic="test";
  msg.payload="hello from " + _device + " - " + _location;
  mqtt_manager->Publish(msg);


  return true;
}


void App::addSensor(sensor* newSensor){
    Serial.print("adding sensor ");
    Serial.print(newSensor->getType().c_str());
    Serial.print(" location: ");
    Serial.println(newSensor->getLocation().c_str());
    
    sensors.push_back(newSensor);
}




bool App::Tick() {

  wifi_manager_->MaybeReconnect();


  mqtt_buffer_->Tick();

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println("reading loop");
    digitalWrite(LED_BUILTIN, HIGH); 
    // thisSensor.read(mqtt_buffer_);
    // thisSensor->read(mqtt_buffer_);

    // for ( thissensor : sensors){
    //   Serial.print ("reading sensor: ");
    //   Serial.print(thissensor->getType().c_str());
    //   Serial.print("-");
    //   Serial.println(thissensor->getLocation().c_str());
    //   thissensor->read(mqtt_buffer_);
    // }

    std::vector<sensor*>::iterator it = sensors.begin();
      while(it != sensors.end()){
      // (*it++)->execute();
      Serial.print ("reading sensor: ");
      Serial.print((*it)->getType().c_str());
      Serial.print("-");
      Serial.println((*it)->getLocation().c_str());
      (*it++)->read(mqtt_buffer_);
    }
    digitalWrite(LED_BUILTIN, LOW); 
    // for (auto it = sensors.begin(); it != sensors.end(); ++it){
    //   // (*it++)->execute();
    //   Serial.print ("reading sensor: ");
    //   Serial.print((*it++)->getType().c_str());
    //   Serial.print("-");
    //   Serial.println((*it++)->getLocation().c_str());

    //   (*it++)->read(mqtt_buffer_);
    // }

    // }

    startMillis = currentMillis; 
  }
  // GetReadings();
  // display_->Clear();
  // window_->Render(display_);
  // display_->Refresh();
  return true;
}

}  // namespace app
}  // namespace deskmate