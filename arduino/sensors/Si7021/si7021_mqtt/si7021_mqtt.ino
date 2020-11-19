#include <WiFi.h>
#include <PubSubClient.h>

#include "Adafruit_Si7021.h"

char ssid[] = "VM9729091";
char pass[] = "bnb3cwmvNhXh";
const char* mqtt_server = "192.168.0.63";

WiFiClient espClient;
PubSubClient client(espClient);


bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_Si7021 sensor = Adafruit_Si7021();

float temperature = 0;
float humidity = 0;
const int ledPin = 2;

void setup() {
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);

  // wait for serial port to open
  while (!Serial) {
    delay(10);
  }

  Serial.println("Si7021 test!");
  
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (true)
      ;
  }

  Serial.print("Found model ");
  switch(sensor.getModel()) {
    case SI_Engineering_Samples:
      Serial.print("SI engineering samples"); break;
    case SI_7013:
      Serial.print("Si7013"); break;
    case SI_7020:
      Serial.print("Si7020"); break;
    case SI_7021:
      Serial.print("Si7021"); break;
    case SI_UNKNOWN:
    default:
      Serial.print("Unknown");
  }
  Serial.print(" Rev(");
  Serial.print(sensor.getRevision());
  Serial.print(")");
  Serial.print(" Serial #"); Serial.print(sensor.sernum_a, HEX); Serial.println(sensor.sernum_b, HEX);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  digitalWrite(ledPin, HIGH);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(ledPin, LOW);
    delay(250);
    
  }
  digitalWrite(ledPin, LOW);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    digitalWrite(ledPin, HIGH);
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
//      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      digitalWrite(ledPin, LOW);
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();

  temperature=sensor.readTemperature();

  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);    
  humidity=sensor.readHumidity();
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  
  Serial.print("Humidity:    ");
  Serial.print(humString);
  Serial.print("\tTemperature: ");
  Serial.println(tempString);

  digitalWrite(ledPin, HIGH);
  client.publish("home/tele/temperature/bedroom", tempString);
  client.publish("home/tele/humidity/bedroom", humString);
  digitalWrite(ledPin, LOW);
  delay(5000);

 
}
