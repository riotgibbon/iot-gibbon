
mosquitto_sub -h 192.168.0.46 -t test


OO access to sensors

basic method

deskmate/src/deskmate/arduino/sensors/sensor.h
`virtual void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer);`

deskmate/src/deskmate/arduino/sensors/si7021.h
`virtual void read(deskmate::mqtt::MQTTMessageBuffer *mqtt_buffer) ;`


deskmate/src/deskmate/app/app.h
void addSensor(sensor newSensor);
std::vector<sensor> sensors;


deskmate/src/deskmate/app/app.cc
void App::addSensor(sensor newSensor) {
    // thisSensor=newSensor;
    Serial.println("adding sensor");
    sensors.push_back(newSensor);
  }


bool App::Tick() {
    for ( thissensor:sensors){
      Serial.println ("reading sensor");
      thissensor.read(mqtt_buffer_);
    }