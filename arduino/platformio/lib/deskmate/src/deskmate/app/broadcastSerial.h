#ifndef BROADCASTSERIAL_H
#define BROADCASTSERIAL_H

#include "broadcast.h"
namespace deskmate {
namespace app {

class broadcaster : public Broadcast {
public:
    // Constructor
    broadcaster(std::string device_name):Broadcast(device_name){
        Serial.println("BroadcastSerial Constructor");
    }

    // Destructor
    ~broadcaster(){
        Serial.println("BroadcastSerial Destructor");
    };

    // Override the broadcast function


    // Override the sendMessage function
    void sendMessage(std::string key, std::string  message) override;
};;
}}
#endif // BROADCASTSERIAL_H