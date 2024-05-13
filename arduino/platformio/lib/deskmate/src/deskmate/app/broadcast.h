#ifndef BROADCAST_H
#define BROADCAST_H

#include <string>
#include "Arduino.h"

namespace deskmate {
namespace app {

class Broadcast {
public:
    virtual void sendMessage(std::string key, std::string  message) = 0;
    Broadcast( std::string device_name) {
        _device_name = device_name;
    }
    virtual ~Broadcast( ) {}
protected:
    std::string _device_name;
};
}}
#endif // BROADCAST_H
