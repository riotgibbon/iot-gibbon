#include "deskmate/arduino/net/wifiMulti.h"


#include <WiFiMulti.h>

#define XSTR(x) #x
#define STR(x) XSTR(x)



namespace deskmate {
namespace arduino {
namespace net {

namespace {
constexpr int kMaxWaitForConnectionMS = 1000;
constexpr int kConnectionLoopDelay = 500;
constexpr int kConnectionTries = 10;



}  // namespace


bool WiFiManager::Connect() {


    Serial.println("Connecting Wifi...");
    if(wifiMulti_.run() == WL_CONNECTED) {
      Serial.print("\nWiFi connected, SSID: ");
      Serial.print(WiFi.SSID());
      Serial.print(", IP address: ");
      Serial.println(WiFi.localIP()); 
      return true;
    }
    else{
        Serial.println("WiFi not connected");
        return false;
    }
}


bool WiFiManager::MaybeReconnect() {
    static uint32_t checkstatus_timeout  = 1000;
    while(WiFi.status()  != WL_CONNECTED) {
        Serial.println("WiFi not connected! Trying to reconnect...");
        Connect();
        // 
        delay(100);
    }
    return WiFi.status() == WL_CONNECTED;
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
