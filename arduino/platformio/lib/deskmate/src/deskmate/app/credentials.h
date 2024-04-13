#ifndef ROOT_DESKMATE_CREDENTIALS_H
#define ROOT_DESKMATE_CREDENTIALS_H

#include <string>
#include <vector>

#include "deskmate/app/config.h"

// TODO: Make these configs dynamic. Ideally configured from a web page, like
// Tasmota/ESPHome.
namespace deskmate {
namespace credentials {
namespace {
using deskmate::app::MQTTConfig;
}  // namespace

// Units.
// Our Arduino drawing library uses the CP437 character set.
constexpr char kUnitDegreeCelcius[] =
    "\xf8"
    "C";

// // WiFi.
// constexpr char kWIFISSID[] = "VM9729091";
// constexpr char kWIFIPassword[] = "bnb3cwmvNhXh";

// WiFi. - hotspot
constexpr char kWIFISSID[] = "GibbonNet";
constexpr char kWIFIPassword[] = "honeybunny";

constexpr char kMQTTGUID[] = "ed33f5b2-7d08-4278-b2b0-06446de0f95d";
// // MQTT.
// constexpr char kMQTTUser[] = "";
// constexpr char kMQTTPassword[] = "";
// constexpr char kMQTTClientId[] = "esp-arduino";
// constexpr char kMQTTServer[] = "192.168.0.46";
// constexpr int kMQTTPort = 1883;


// MQTT.
constexpr char kMQTTUser[] = "";
constexpr char kMQTTPassword[] = "";
constexpr char kMQTTClientId[] = "esp-arduino";
constexpr char kMQTTServer[] = "broker.hivemq.com";
constexpr int kMQTTPort = 1883;


// // // MQTT. hiveMQ TLS
// constexpr char kMQTTUser[] = "mobile";
// constexpr char kMQTTPassword[] = "Po11ution";
// constexpr char kMQTTClientId[] = "esp-arduino";
// constexpr char kMQTTServer[] = "4f67a8b4e1c9480997bf9fa8ce9378d7.s1.eu.hivemq.cloud";
// constexpr int kMQTTPort = 8883;


// MQTT.
// constexpr char kMQTTUser[] = "";
// constexpr char kMQTTPassword[] = "";
// constexpr char kMQTTClientId[] = "esp-arduino";
// constexpr char kMQTTServer[] = "192.168.0.44";
// constexpr int kMQTTPort = 1883;

// Display pins.
constexpr uint8_t kSCKPin = 14;
constexpr uint8_t kMOSIPin = 13;
constexpr uint8_t kCSPin = 15;

// Display size.
constexpr int kDisplayHeight = 240;
constexpr int kDisplayWidth = 400;

// Input pins.
// Crank A & B pins are the rotary encoder's two phase-shifted input pins.
constexpr uint8_t kButtonAPin = 27;
constexpr uint8_t kButtonBPin = 26;
constexpr uint8_t kButtonCPin = 25;
constexpr uint8_t kCrankAPin = 33;
constexpr uint8_t kCrankBPin = 34;
constexpr uint8_t kCrankPushPin = 32;

// MQTT-powered switches.
const std::vector<MQTTConfig> kMQTTConfigs{
    {"Desk", "home/switch/cmnd/officedesk/POWER",
     "home/switch/stat/officedesk/POWER"},
    {"Office", "home/switch/cmnd/office/POWER",
     "home/switch/stat/office/POWER"},
};

// MQTT-powered floating point sensors.
const std::vector<deskmate::app::MQTTFloatingPointSensorConfig>
    kMQTTFloatingPointSensors{
        {"Calethea", "home/Calethea/soil_moisture_percent/retained",
         "home/Calethea/availability", "%"},
        {"ChineseLamp", "home/ChineseLamp/soil_moisture_percent/retained",
         "home/ChineseLamp/availability", "%"},
    };

// MQTT-powered weather configs.
const std::vector<deskmate::app::MQTTFloatingPointSensorConfig>
    kMQTTWeatherConfigs{
        {"Zurich", "home/Zurich/temp/retained", "home/Zurich/availability",
         kUnitDegreeCelcius},
        {"Office", "ble_bridge/sensor/office_temperature/state",
         "ble_bridge/sensor/office_temperature/state/availability",
         kUnitDegreeCelcius},
    };

}  // namespace credentials
}  // namespace deskmate

#endif  // ROOT_DESKMATE_CREDENTIALS_H