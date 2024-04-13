#include "deskmate/arduino/net/mqtt.h"
#include "deskmate/app/credentials.h"

#include <Arduino.h>
#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif


#include <FS.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>


#include <algorithm>



namespace deskmate {
namespace arduino {
namespace net {


namespace {
using deskmate::mqtt::MQTTMessage;
using deskmate::mqtt::MQTTSubscriber;
using deskmate::credentials::kMQTTGUID;

// Max that PubSubClient supports.
constexpr int kSubscriptionQoS = 1;
}  // namespace


// MQTTManager::MQTTManager(){

// };

MQTTManager::MQTTManager(const char* server, int port, const char* username,
                         const char* password, const char* client_id)
    : username_(username), password_(password), client_id_(client_id) {
      

      bool isTLS = false;

      if (isTLS) {
        Serial.println("TLS enabled");

      WiFiClientSecure espClient;
      LittleFS.begin();
      BearSSL::CertStore certStore;

      Serial.println("creating PubSubClient");
      int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
    Serial.printf("Number of CA certs read: %d\n", numCerts);
    if (numCerts == 0) {
      Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
      return; // Can't connect to anything w/o certs!
    }

    BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
    // Integrate the cert store with this conection
    bear->setCertStore(&certStore);

    pubsub_client_ = new PubSubClient(server,port, *bear);

    // pubsub_client_->setServer(server, port);

      } else {
        Serial.println("TLS disabled");
        pubsub_client_= new PubSubClient(server,port, wifi_client_);
      }
      // pubsub_client_= std::make_unique<PubSubClient>(server,port, wifi_client_);
      // 

      pubsub_client_->setBufferSize(2048);
      Serial.println("created PubSubClient");
  // Register the "On new message" callback, which calls Dispatch.
  // No fancy synchronization is needed here, since this callback only
  // runs when we call loop() in our main... loop. In other works, there
  // is no other thread.
  // pubsub_client_ = std::make_unique<PubSubClient>(
  //     server, port,
  //     [this](const char* topic, byte* payload,  int length) {
  //       std::string str_payload =
  //           std::string(reinterpret_cast<char*>(payload), length);
  //       Serial.print("[mqtt] Got message: %s -> %s\n");
  //       Serial.print(topic);
  //       Serial.print(str_payload.c_str());
  //       for (MQTTSubscriber* subs : this->subscribers_by_topic_[topic]) {
  //         subs->HandleMessage({topic, str_payload});
  //       }
  //     },
  //     wifi_client_);
  
}


bool MQTTManager::Connect() {

  if (!IsConnected()) {
    Serial.println(client_id_.c_str());
    Serial.println(username_.c_str());
    Serial.println(password_.c_str());
    Serial.println("[mqtt] Attempting connection.");

    pubsub_client_->connect(client_id_.c_str(), username_.c_str(),   password_.c_str());
    return IsConnected();
  }
  return IsConnected();
}

bool MQTTManager::IsConnected() const { return pubsub_client_->connected(); }

bool MQTTManager::Subscribe(MQTTSubscriber* subscriber) {
  for (const std::string& topic : subscriber->SubscriptionTopics()) {
    subscribers_by_topic_[topic].push_back(subscriber);
    pubsub_client_->subscribe(topic.c_str(), kSubscriptionQoS);
  }
  return true;
}

// PubSubClient::publish only supports QoS = 0.
bool MQTTManager::Publish(const MQTTMessage& msg) {
  out_queue_.push(msg);
  return true;
}

bool MQTTManager::Tick() {
  pubsub_client_->loop();

  if (!IsConnected()) {
    Serial.println("[mqtt] Connection lost. Reconnecting.");
    if (!Connect()) {
      Serial.println("[mqtt] Unable to reconnect.");
      return false;
    }

    std::for_each(
        subscribers_by_topic_.cbegin(), subscribers_by_topic_.cend(),
        [this](
            const std::pair<std::string, std::vector<MQTTSubscriber*>>& pair) {
          return pubsub_client_->subscribe(pair.first.c_str(),
                                           kSubscriptionQoS);
        });
  }

  // Send out messages.
  while (!out_queue_.empty()) {
    //  delay(500);
    const MQTTMessage& msg = out_queue_.front();
    Serial.print("[mqtt] Publishing message:  ");
    // Generate a GUID
    // std::string guid = mqttGUID;

    // Append the GUID to msg.topic
    std::string topic= msg.topic;
    
    if (sizeof(kMQTTGUID) > 0) {
      std::string guid;
      guid += kMQTTGUID;
      topic = guid + "/" + msg.topic;  
    }
    // std::string topicWithGUID = mqttGUID.append("/").append(msg.topic);

    // Publish the message with the updated topic

    Serial.print( topic.c_str());
    Serial.print( " : ");
    Serial.println( msg.payload.c_str());

    if (!pubsub_client_->publish(topic.c_str(), msg.payload.c_str())) {
      Serial.println("[mqtt] Error sending message %s -> %s\n");
      Serial.print( topic.c_str());
      Serial.print( " : ");
      Serial.println( msg.payload.c_str());
      delay(50);
      // out_queue_.push(msg);
    }
    // }else {
    //   out_queue_.pop();
    //   }

    out_queue_.pop();
    delay(50);
  }
  
  return true;
}

}  // namespace net
}  // namespace arduino
}  // namespace deskmate
