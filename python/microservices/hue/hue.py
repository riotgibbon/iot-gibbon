import paho.mqtt.client as mqtt
import time
from datetime import datetime,timedelta
import traceback
import requests
from phue import Bridge  # https://github.com/studioimaginaire/phue
from influxdb import InfluxDBClient


hueDry = 65535
hueWet = 0
min = 800
max = 840

plants = ['yucca', 'amaryllis', 'bonsai', 'aralia']
plantTopics ="home/tele/soilmoisture/livingroom/"

transitionTime = 20

plantSeconds=60
currentPlantCount=0


def getPlantChangeTime():
    return datetime.now() + timedelta(0,plantSeconds)


plantChangeTime= getPlantChangeTime()
print(f"plantChangeTime: {plantChangeTime}")

def getInfluxClient(host='localhost', port=8086):
    user=''
    password=''
    dbname = 'home'
    client = InfluxDBClient(host, port, user, password, dbname)
    return client

def getValue(client, reading):
    query =  f"SELECT MEAN(value) FROM mqtt_consumer   WHERE time > now() - 30s  and topic = 'home/tele/{reading}/livingroom/window'"
    result = client.query(query)
    value = int(list(result.get_points(measurement='mqtt_consumer'))[0]['mean'])
    return value

host = '192.168.0.14'

b = Bridge(host)

# If the app is not registered and the button is not pressed, press the button and call connect() (this only needs to be run a single time)
b.connect()

# Get the bridge state (This returns the full dictionary that you can explore)
b.get_api()

influxClient = getInfluxClient()


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

def mapRange( x,  in_min,  in_max,  out_min,  out_max):
  return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)



def postToLights(reading):
    
    key='Zk16ZQhoxu1MHAJskpApN8i-y8xg0EfGULyBMHS7'
    lightId = 7

    mapped = mapRange(reading,min,max,hueWet,hueDry)

    try:


        temperature = getValue(influxClient, 'temperature')
        mappedTemperature= mapRange(temperature,15,35,230,254)
        print(f"temp: {temperature}C, mapped: {mappedTemperature}")
        # b.set_light(lightId, 'sat', mappedTemperature)
        humidity = getValue(influxClient, 'humidity')
        mappedHumidity = mapRange(humidity,60,100,200,254)
        print(f"humidity: {humidity}%, mapped: {mappedHumidity}")
        # b.set_light(lightId, 'bri', mappedHumidity) 
        
        command =  {'transitiontime' : transitionTime,  'hue':  mapped, 'sat':mappedTemperature, 'bri': mappedHumidity}
        print(command)
        b.set_light(lightId,command)
        b.set_light(2,command)
        lightInfo= str(b.get_light(lightId))

        client.publish('home/cmd/hue/tv/', lightInfo)
    except Exception:
        print ("error posting hue data")
        traceback.print_exc()


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(f"{plantTopics}#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # print(f"new message")
    global plantChangeTime 
    global currentPlantCount 
    print(f"new message {msg.topic}: {str(msg.payload)}")
    plantCount=len(plants)

    if datetime.now() > plantChangeTime:
        plantChangeTime = getPlantChangeTime()
        currentPlantCount+=1
        

    currentPlantIndex = currentPlantCount % plantCount
    currentPlantName = plants[currentPlantIndex]

    if currentPlantTopic == str(msg.topic):
        reading =int(msg.payload.decode("utf-8"))
        print (f"currentPlantCount: {currentPlantCount}, currentPlantIndex: {currentPlantIndex}, currentPlantName: {currentPlantName} ")
        currentPlantTopic = f"{plantTopics}{currentPlantName}"
        if reading>min and reading<max:
            nextPlantTime=plantChangeTime - datetime.now()
            print(f"processing {currentPlantName}: {reading}, next plant in {nextPlantTime}")
            postToLights(reading)
        


client = getMqttClient()
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
