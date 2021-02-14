#include "deskmate/arduino/net/wifi.h"


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



namespace deskmate {
namespace arduino {
namespace net {

namespace {
constexpr int kMaxWaitForConnectionMS = 1000;
constexpr int kConnectionLoopDelay = 500;
constexpr int kConnectionTries = 10;

bool WiFiTryToConnectOnce(const char* ssid, const char* password) {
   long delayed = 0;
  WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //  https://github.com/esp8266/Arduino/issues/119#issuecomment-421530346 

  while (WiFi.waitForConnectResult() != WL_CONNECTED){

    // Serial.print("[wifi] WiFi.status():", + WiFi.status());
    delay(kConnectionLoopDelay);
    delayed += kConnectionLoopDelay;
    if (delayed > kMaxWaitForConnectionMS) {
      return false;
    }
  }
  Serial.print("[wifi] Connected!\n");
  return true;
}

}  // namespace

// For some reason, my uc (esp32) fails to connect every other time. This
// re-begin() approach seems to work, although it's not very  elegant.
bool WiFiManager::Connect() {



  for (int i = 0; i < kConnectionTries; i++) {
    // Serial.print("[wifi] Attempting to connect %d/%d...\n", i,   kConnectionTries);
    if (WiFiTryToConnectOnce(ssid_, password_)) {
      return true;
    }
  }
  Serial.print("[wifi] Unable to connect.\n");
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
