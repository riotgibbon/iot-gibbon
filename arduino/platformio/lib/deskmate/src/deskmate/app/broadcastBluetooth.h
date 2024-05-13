#ifndef BROADCASTBLUETOOTH_H
#define BROADCASTBLUETOOTH_H

#include "broadcast.h"
#include "BluetoothSerial.h"
namespace deskmate {
namespace app {

class broadcaster : public Broadcast {
public:
    
    broadcaster(std::string device_name):Broadcast(device_name){
        Serial.print("broadcastBluetooth Constructor:");
        Serial.println(device_name.c_str());
        init();
    }

    // Destructor// Constructor
    ~broadcaster(){
        Serial.println("broadcastBluetooth Destructor");
    };

    // Override the broadcast function


    // Override the sendMessage function
    void sendMessage(std::string key, std::string  message) override;
 private:
    void init();
    void bluetoothCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
    bool BT_connected = true;
    BluetoothSerial SerialBT;
    String bluetoothReadLine();
};;
}}
#endif // BROADCASTBLUETOOTH_H