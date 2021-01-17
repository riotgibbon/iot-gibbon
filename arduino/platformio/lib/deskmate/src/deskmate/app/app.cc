#include "deskmate/app/app.h"
#include <Arduino.h>

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

// using deskmate::arduino::sensors::dummy;
// using deskmate::arduino::sensors::si7021;
// using deskmate::arduino::sensors::sensor;




unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds

std::vector<sensor* > sensors;


}  // namespace



bool App::Init(
    const std::vector<MQTTConfig> &mqtt_configs,
    const std::vector<MQTTFloatingPointSensorConfig> &sensor_configs,
    const std::vector<MQTTFloatingPointSensorConfig> &weather_configs) {

    // InitSensor();
    // thisSensor = si7021("bedroom");
    startMillis = millis();  //initial start time

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
  mqtt_buffer_->Tick();

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println("reading loop");
    // thisSensor.read(mqtt_buffer_);
    // thisSensor->read(mqtt_buffer_);
    for ( thissensor:sensors){
      Serial.print ("reading sensor: ");
      Serial.print(thissensor->getType().c_str());
      Serial.print("-");
      Serial.println(thissensor->getLocation().c_str());
      thissensor->read(mqtt_buffer_);
    }

// vector<Command*>::iterator it = commands.begin();
// while(it != commands.end())
// (*it++)->execute();
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