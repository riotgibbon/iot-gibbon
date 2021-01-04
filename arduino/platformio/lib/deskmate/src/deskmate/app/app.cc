#include "deskmate/app/app.h"

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

#include "Adafruit_Si7021.h"

namespace deskmate {
namespace app {
namespace {
using deskmate::app::MQTTConfig;
using deskmate::app::MQTTFloatingPointSensorConfig;
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTMessageBuffer;



Adafruit_Si7021 sensor = Adafruit_Si7021();
float temperature = 0;
float humidity = 0;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds


}  // namespace


bool InitSensor(){
  Serial.println("Si7021 test!");
  
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    return false;
  }

  Serial.print("Found model ");
  switch(sensor.getModel()) {
    case SI_Engineering_Samples:
      Serial.print("SI engineering samples"); break;
    case SI_7013:
      Serial.print("Si7013"); break;
    case SI_7020:
      Serial.print("Si7020"); break;
    case SI_7021:
      Serial.print("Si7021"); break;
    case SI_UNKNOWN:
    default:
      Serial.print("Unknown");
  }
  Serial.print(" Rev(");
  Serial.print(sensor.getRevision());
  Serial.print(")");
  Serial.print(" Serial #"); Serial.print(sensor.sernum_a, HEX); Serial.println(sensor.sernum_b, HEX);
  return true;
}

bool App::Init(
    const std::vector<MQTTConfig> &mqtt_configs,
    const std::vector<MQTTFloatingPointSensorConfig> &sensor_configs,
    const std::vector<MQTTFloatingPointSensorConfig> &weather_configs) {

    // InitSensor();
    startMillis = millis();  //initial start time

  return true;
}


void GetReadings(){
    temperature=sensor.readTemperature();
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);    
  humidity=sensor.readHumidity();
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  
  Serial.print("Humidity:    ");
  Serial.print(humString);
  Serial.print("\tTemperature: ");
  Serial.println(tempString);

}

void App::dummyReading(){
  
  
  MQTTMessage msg;
  msg.topic="test";
  msg.payload="reading to go here";

  // Serial.println(msg.payload);
  mqtt_buffer_->Publish(msg);
}

bool App::Tick() {
  mqtt_buffer_->Tick();

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println("reading loop");
    dummyReading();
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