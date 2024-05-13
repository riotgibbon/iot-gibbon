#include "deskmate/app/broadcastBluetooth.h"

#include <iostream>
#include <string>

#include "BluetoothSerial.h"


namespace deskmate {
namespace app {

void broadcaster::sendMessage(std::string key, std::string  message){
    // Code to send the message via serial communication
    Serial.print("Key: ");
    Serial.print(key.c_str());
    Serial.print(" Message: ");
    Serial.println(message.c_str());
    if (BT_connected)            {
        Serial.print("sending data via Bluetooth:");
        SerialBT.print(key.c_str());
        SerialBT.print('|');               //Value Separator              
        SerialBT.println(message.c_str());
        Serial.println("Sent data");
    }
    else
    {
        Serial.println("Bluetooth not connected");
    }
}

String broadcaster::bluetoothReadLine()
{
    String response = "";
    while(SerialBT.available())
    {
        byte r = SerialBT.read();
        if(r!=13 && r!=10 && char(r)!='\0')
            response = response + char(r);
    }
    return response;      
}

void broadcaster::bluetoothCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    if(event == ESP_SPP_SRV_OPEN_EVT)
    {
        Serial.println("Connection established");
        SerialBT.printf("Hello from |\"%s\"!\n", _device_name.c_str());
        BT_connected = true;
    }
    else if(event == ESP_SPP_CLOSE_EVT)
    {
        Serial.println("Connection closed");
//        BT_connected = false;
//        ESP.restart();
    }
    // else if(event == ESP_SPP_DATA_IND_EVT)
    // {
    //     Serial.println("Data received");
    //     String response = bluetoothReadLine();
    //     if(response=="")
    //     {
    //         Serial.println("EMPTY");
    //     }
    // }
    else if(event == ESP_SPP_WRITE_EVT)
    {
        Serial.println("Write operation complete");
    }
} 

void broadcaster::init(){
    Serial.println("Initializing Bluetooth");
    // SerialBT = new BluetoothSerial();
    // SerialBT.register_callback(bluetoothCallback);
  SerialBT.begin(_device_name.c_str()); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", _device_name);

}


}}