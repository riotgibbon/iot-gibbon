#ifndef DESKMATE_APP_APP_H
#define DESKMATE_APP_APP_H

#include <iostream>
#include <memory>
#include <vector>

#include "deskmate/app/config.h"
// #include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/window.h"
#include "deskmate/input/input.h"
#include "deskmate/mqtt/mqtt.h"

#include "deskmate/arduino/sensors/si7021.h"

namespace deskmate {
namespace app {
namespace {

using deskmate::app::MQTTConfig;
using deskmate::arduino::sensors::si7021;
// using deskmate::gfx::Display;
}  // namespace

  // struct readings
  // {
  //   std::string temperature;
  //   std::string humidity;
  // };

// TODO: make this more flexible. In an ideal world, it would receive a config
// file and would instantiate an app to match it. Right now the definitions of
// which screens, items are used are hardcoded, and the configs only control
// which items are present.
// Clients of this class should be able to instantiate any combination of
// screen/items they want.
class App {
 public:
  App( deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer)
      :  mqtt_buffer_(mqtt_buffer) {}

  bool Init(const std::vector<MQTTConfig> &mqtt_configs,
            const std::vector<MQTTFloatingPointSensorConfig> &sensor_configs,
            const std::vector<MQTTFloatingPointSensorConfig> &weather_configs);

  bool Tick();
  
  // readings GetReadings();


 private:
  // void dummyReading(); 
  deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer_;
  si7021 sensor_si7021;

};

}  // namespace app
}  // namespace deskmate

#endif  // DESKMATE_APP_APP_H