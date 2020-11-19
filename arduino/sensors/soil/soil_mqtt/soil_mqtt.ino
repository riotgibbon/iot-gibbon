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

char ssid[] = "VM9729091";
char pass[] = "bnb3cwmvNhXh";
char host[] = "192.168.0.63";
char clientid[] = "arduinoMKR1010";
char topicname[] = "home/tele/soilmoisture/livingroom/yucca";

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
// client.subscribe("/hello"); //SUBSCRIBE TO TOPIC /hello
}



int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 7;//Variable for Soil moisture Power



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  //
//  while (!Serial) {
//    delay(1000);
//  }
  //
  WiFi.begin(ssid, pass);
  delay(10000);

  client.begin(host, 1883, net);

  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

}

void loop() {
  char cstr[16];
  int   soilValue = readSoil();
   char* soil = itoa(soilValue, cstr, 10);
  Serial.print("Soil Moisture: "); 
  Serial.println(soil);

  client.loop();
  if (!client.connected()) {
   connect();
  }

  digitalWrite(LED_BUILTIN, HIGH); 
  
 
  client.publish(topicname, soil);

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


int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}
