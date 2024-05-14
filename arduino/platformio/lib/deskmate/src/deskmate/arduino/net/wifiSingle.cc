#include "deskmate/arduino/net/wifiSingle.h"


#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#undef ESP8266 
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#undef ESP8266 
#elif ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#define XSTR(x) #x
#define STR(x) XSTR(x)



namespace deskmate {
namespace arduino {
namespace net {

namespace {
constexpr int kMaxWaitForConnectionMS = 1000;
constexpr int kConnectionLoopDelay = 500;
constexpr int kConnectionTries = 10;


#ifdef ARDUINO_SAMD_MKRWIFI1010
bool WifiNotConnected(){
  Serial.println("ARDUINO_SAMD_MKRWIFI1010");
  return WiFi.status() != WL_CONNECTED;
}
#elif ARDUINO_SAMD_MKR1000
bool WifiNotConnected(){
  Serial.println("ARDUINO_SAMD_MKR1000");
  return WiFi.status() != WL_CONNECTED;
}

#elif ESP8266
bool WifiNotConnected(){
  Serial.println("ESP8266");
  return WiFi.waitForConnectResult()!= WL_CONNECTED;
}


#else
bool WifiNotConnected(){
  Serial.println("wifi.h");
  return WiFi.waitForConnectResult()!= WL_CONNECTED;
}
#endif




bool WiFiTryToConnectOnce() {

  char  WIFI_SSID[] = STR(F_WIFI_SSID);
  char  WIFIPassword[] = STR(F_WIFIPassword);
  Serial.print("Connecting to : ");  
  Serial.println(WIFI_SSID);

  long delayed = 0;
  WiFi.begin(WIFI_SSID, WIFIPassword);
  Serial.print("[wifi] library: ");
  while (WifiNotConnected()){
    Serial.println("[wifi] connecting ...");
    delay(kConnectionLoopDelay);
    delayed += kConnectionLoopDelay;
    if (delayed > kMaxWaitForConnectionMS) {
      return false;
    }
  }
    Serial.println   ("[wifi] Connected!");
  return true;
  
}


}  // namespace

// For some reason, my uc (esp32) fails to connect every other time. This
// re-begin() approach seems to work, although it's not very  elegant.
bool WiFiManager::Connect() {
  for (int i = 0; i < kConnectionTries; i++) {
    if (WiFiTryToConnectOnce()) {
      return true;
    }
    Serial.println("[wifi] retrying!");
  }
  Serial.println("[wifi] Unable to connect.");
  return false;
}

// TODO: do we need the hacky retries here too?
bool WiFiManager::MaybeReconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    return Connect();
  }
  return true;
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
