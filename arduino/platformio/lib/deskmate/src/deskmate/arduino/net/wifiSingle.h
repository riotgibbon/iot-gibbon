#ifndef DESKMATE_ARDUINO_NET_WIFI_H
#define DESKMATE_ARDUINO_NET_WIFI_H

#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#define WIFI_SINGLE 
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#define WIFI_SINGLE 
#elif ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#define WIFI_MULTI
#else
#include <WiFi.h>
#include <WiFiMulti.h>
#define WIFI_MULTI
#endif


// #include "deskmate/arduino/net/defines.h"
// #include "WiFiMulti_Generic.h"
namespace deskmate {
namespace arduino {
namespace net {

class WiFiManager {
 public:
  WiFiManager(){}
  // WiFiManager(const char* ssid, const char* password)
  //     : ssid_(ssid), password_(password) {}
  bool Connect();
  bool MaybeReconnect();

 private:

  const char* ssid_;
  const char* password_;
};

}  // namespace net
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_NET_WIFI_H