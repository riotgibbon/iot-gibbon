#include "deskmate/app/app.h"

// #include "deskmate/arduino/sensors/dummy.h"
// #include "deskmate/arduino/sensors/si7021.h"

#include "deskmate/app/config.h"
#include "deskmate/gfx/components/mqtt_circle_horizontal_list_item.h"
#include "deskmate/gfx/components/mqtt_list_item.h"
#include "deskmate/gfx/components/mqtt_vertical_bar_horizontal_list_item.h"
#include "deskmate/gfx/components/text_list_item.h"
#include "deskmate/gfx/display.h"
#include "deskmate/gfx/screens/horizontal_list.h"
#include "deskmate/gfx/screens/list.h"
#include "deskmate/gfx/screens/window.h"
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
using deskmate::arduino::sensors::si7021;
using deskmate::arduino::sensors::sensor;


// Adafruit_Si7021 sensor = Adafruit_Si7021();

// float temperature = 0;
// float humidity = 0;

// si7021 sensor_si7021;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds



}  // namespace



bool App::Init(
    const std::vector<MQTTConfig> &mqtt_configs,
    const std::vector<MQTTFloatingPointSensorConfig> &sensor_configs,
    const std::vector<MQTTFloatingPointSensorConfig> &weather_configs) {

    // InitSensor();
    sensor_si7021 = si7021("bedroom");
    startMillis = millis();  //initial start time

  return true;
}


// readings App::GetReadings(){
//     temperature=sensor.readTemperature();
//   char tempString[8];
//   dtostrf(temperature, 1, 2, tempString);    
//   humidity=sensor.readHumidity();
//   char humString[8];
//   dtostrf(humidity, 1, 2, humString);
  
//   Serial.print("Humidity:    ");
//   Serial.print(humString);
//   Serial.print("\tTemperature: ");
//   Serial.println(tempString);

//   readings thisReading;
//   thisReading.temperature=tempString;
//   thisReading.humidity=humString;



//     MQTTMessage temperatureMessage;
//     temperatureMessage.topic ="test/tele/temperature/kitchen";
//     temperatureMessage.payload=thisReading.temperature;

//     MQTTMessage humidityMessage;
//     humidityMessage.topic="test/tele/humidity/kitchen";
//     humidityMessage.payload =thisReading.humidity;  

//     mqtt_buffer_->Publish(temperatureMessage);
//     mqtt_buffer_->Publish(humidityMessage);
    

//   return thisReading;

// }

// void App::dummyReading(){
  
  
//   MQTTMessage msg;
//   msg.topic="test";
//   msg.payload="reading to go here";

//   // Serial.println(msg.payload);
//   mqtt_buffer_->Publish(msg);
// }

bool App::Tick() {
  mqtt_buffer_->Tick();

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println("reading loop");
    sensor_si7021.read(mqtt_buffer_);
    // readings thisReading = GetReadings();
    // dummy thisDummy = dummy();
    // thisDummy.read();
    

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