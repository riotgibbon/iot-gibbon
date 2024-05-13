#include "deskmate/app/broadcastSerial.h"

#include <iostream>
#include <string>



namespace deskmate {
namespace app {

void broadcaster::sendMessage(std::string key, std::string  message){
    // Code to send the message via serial communication
    Serial.print("Key: ");
    Serial.print(key.c_str());
    Serial.print(" Message: ");
    Serial.println(message.c_str());
}
}}