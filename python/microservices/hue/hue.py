import paho.mqtt.client as mqtt
import time
from datetime import datetime,timedelta
import traceback
import requests

def getMqttClient():
    client = mqtt.Client()
    while (True):
        try:
            client.connect("192.168.0.63", 1883, 60)
            break
        except Exception:
            print ("error connecting, pausing")
            traceback.print_exc()
            time.sleep(5)
    return client

def getHue(reading):
    # make range between 810 and 850
    min = 800
    max = 850

    readingPc = (reading-min)/(max-min)

    hueDry = 65535
    hueWet = 45000

    hueRange = hueDry-hueWet

    hueReading = int(((1-readingPc)* hueRange) + hueWet)

    return hueReading


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("home/tele/soilmoisture/livingroom/yucca")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # print(f"new message")
    print(f"new message {msg.topic}: {str(msg.payload)}")
    reading =int(msg.payload.decode("utf-8"))
    hueReading = getHue(reading)
    print (f"reading : {reading} = {hueReading}")

client = getMqttClient()
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
