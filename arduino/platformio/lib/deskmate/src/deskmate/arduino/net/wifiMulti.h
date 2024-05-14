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

#define XSTR(x) #x
#define STR(x) XSTR(x)


namespace deskmate {
namespace arduino {
namespace net {

class WiFiManager {
 public:
  WiFiManager(){ 
    wifiMulti_.addAP(STR(F_WIFI_SSID),STR(F_WIFIPassword));
    wifiMulti_.addAP(STR(F_WIFI_SSID_1),STR(F_WIFIPassword_1));
    wifiMulti_.addAP(STR(F_WIFI_SSID_2),STR(F_WIFIPassword_2));
   }

  bool Connect();
  bool MaybeReconnect();

 private:
    WiFiMulti wifiMulti_;
};

}  // namespace net
}  // namespace arduino
}  // namespace deskmate

#endif  // DESKMATE_ARDUINO_NET_WIFI_H