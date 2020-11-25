/*
 * Publishes "Hello" at regular intervals. The message can be subscribed and observed in HiveMQs
 * Websockets client http://www.hivemq.com/demos/websocket-client/
 *
 * Copyright (C) 2018 Andreas Motzek andreas-motzek@t-online.de
 *
 * You can use, redistribute and/or modify this file under the terms of the Modified Artistic License.
 * See http://simplysomethings.de/open+source/modified+artistic+license.html for details.
 *
 * This file is distributed in the hope that it will be useful, but without any warranty; without even
 * the implied warranty of merchantability or fitness for a particular purpose.
 */

#include <MQTT.h>
#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#else
#error unknown board
#endif
WiFiClient net;
MQTTClient client;

/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 5
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);



char ssid[] = "VM9729091";
char pass[] = "bnb3cwmvNhXh";
char host[] = "192.168.0.63";
char clientid[] = "arduinoMKR1010";
char yucca_moisture_topic[] = "home/tele/soilmoisture/livingroom/yucca";
char yucca_temperature_topic[] = "home/tele/temperature/livingroom/yucca";
char aralia_temperature_topic[] = "home/tele/temperature/livingroom/aralia";

void connect() {
 Serial.print("checking wifi...");
 while (WiFi.status() != WL_CONNECTED) {
  digitalWrite(LED_BUILTIN, HIGH); 
   Serial.print(".");
   delay(250);
   digitalWrite(LED_BUILTIN, LOW); 
   delay(250);
 }
 Serial.print("\nconnecting...");
 while (!client.connect(clientid)) {
   Serial.print(".");
   delay(1000);
 }
 Serial.println("\nconnected!");

}



int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 7;//Variable for Soil moisture Power



void setup(void) {
  Serial.println("beginning setup"); 
  Serial.begin(9600);

  Serial.println("Starting Dallas Temperature"); 
  // Start up the library 
  sensors.begin(); 
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println("Starting wifi"); 
  WiFi.begin(ssid, pass);
  delay(10000);

  Serial.println("Starting mqtt"); 
  client.begin(host, 1883, net);

  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

  Serial.println("setup complete"); 
}

void loop() {
  char cstr[16];
  int   soilValue = readSoil();
  if (soilValue > 100) {
     char* soil = itoa(soilValue, cstr, 10);
    Serial.print("Soil Moisture: "); 
    Serial.println(soil);
  
    client.loop();
    if (!client.connected()) {
     connect();
    }
  
    digitalWrite(LED_BUILTIN, HIGH); 
    
    
    client.publish(yucca_moisture_topic, soil);


     // call sensors.requestTemperatures() to issue a global temperature 
   // request to all devices on the bus 
  /********************************************************************/
   Serial.print(" Requesting temperatures..."); 
   sensors.requestTemperatures(); // Send the command to get temperature readings 
   Serial.println("DONE"); 
  /********************************************************************/


   readSendTemp("yucca", 0);
   readSendTemp("aralia", 1);

    digitalWrite(LED_BUILTIN, LOW);  
    //
    switch (WiFi.status()) {
      case WL_CONNECT_FAILED:
      case WL_CONNECTION_LOST:
      case WL_DISCONNECTED: WiFi.begin(ssid, pass);
    }
    //
    delay(5000);
  }
}

void readSendTemp(String plant, int index)
{
   String topicBase="home/tele/temperature/livingroom/";
   double temp = sensors.getTempCByIndex(index);
   String temp_str = String(temp, 2);

   Serial.print(plant + "temp is: "); 
   Serial.println(temp_str); // Why "byIndex"?  - get addresses 0 https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/
   String topic = topicBase + plant;
   client.publish(topic, temp_str);
  
}

int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}
