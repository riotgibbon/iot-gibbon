import paho.mqtt.client as mqtt
import time
from datetime import datetime,timedelta
import traceback
import requests
from phue import Bridge  # https://github.com/studioimaginaire/phue
from influxdb import InfluxDBClient
import json
from rgbxy import Converter

converter = Converter()

hueHigh = 65535
hueLow = 0
moistureMin = 800
moistureMax = 840

plants = ['yucca', 'amaryllis', 'bonsai', 'aralia']
plantTopics ="home/tele/soilmoisture/livingroom/"

transitionTime = 20

plantSeconds=180
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

def getWindowAverageValue(client, reading, default):

    try:
        query =  f"SELECT MEAN(value) FROM mqtt_consumer   WHERE time > now() - 30s  and topic = 'home/tele/{reading}/livingroom/window'"
        result = client.query(query)
        value = (list(result.get_points(measurement='mqtt_consumer'))[0]['mean'])
        return value
    except:
        return default

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

def indexValue(value, low, high):
    return mapRange(value, low,high,1,100)

def getPressureIndex(client):
    query =  f"SELECT last(value), min(value), max(value) FROM mqtt_consumer   WHERE  topic = 'home/tele/pressure/livingroom/desk' and value > 970"
    result = client.query(query)
    values = (list(result.get_points(measurement='mqtt_consumer'))[0])
    min= values['min']
    max=values['max']
    last=values['last']
    index = indexValue(last,min,max)
    print (f"pressure: last: {last}, min: {min}, max {max}, index: {index}")
    return index


def postToLights(plantName, reading):
    lightId = 7
    # light mapping:
    # get index of pressure
    pressureIndex = getPressureIndex(influxClient)
    # get index of moisture readings
    #mapped = mapRange(reading,moistureMin,moistureMax,hueLow,hueHigh)
    moistureIndex =  indexValue(reading,moistureMin,moistureMax) 
    # multiple pressure and moisture indexes to
    hueIndex = pressureIndex * moistureIndex

    hueMappedValue =mapRange (hueIndex, 1, 10000, hueLow, hueHigh)
    print(f"moisture: {reading}, pressureIndex: {pressureIndex}, moistureIndex: {moistureIndex}, hueIndex: {hueIndex}, hueMappedValue: {hueMappedValue}")

    try:
        temperature = getWindowAverageValue(influxClient, 'temperature', 22)
        mappedTemperature= mapRange(temperature,15,30,230,254)
        print(f"temp: {temperature:.2f}C, mapped: {mappedTemperature}")
        humidity = getWindowAverageValue(influxClient, 'humidity',60)
        mappedHumidity = mapRange(humidity,60,100,200,254)
        print(f"humidity: {humidity:.2f}%, mapped: {mappedHumidity}")
        
        command =  {'transitiontime' : transitionTime,  'hue':  hueMappedValue, 'sat':mappedTemperature, 'bri': mappedHumidity}
        print(command)
        b.set_light(lightId,command)
        # b.set_light(2,command)

        lightInfo= b.get_light(lightId)

        xy = lightInfo['state']['xy']
        x= xy[0]
        y=xy[1]

        bri =mappedHumidity/254

        # red,,b = converter.xy_to_rgb(x,y,bri)

        

        hex = f"#{converter.xy_to_hex(x,y,bri)}"

        hue= {'xy': xy, 'hex': hex, 'rgb':converter.xy_to_rgb(x,y,bri), 'bri': bri }
        # print(hue)
        body={}
        body['hue']=hue #json.dumps(lightInfo)
        nextPlantTime=(plantChangeTime - datetime.now()).seconds
        plantInfo ={'name':plantName, 'reading': reading, 'nextPlantAt': nextPlantTime}
        body['plant']=plantInfo
        jsonMsg = json.dumps(body)
        print(jsonMsg)
        client.publish('home/cmd/hue/tv', jsonMsg)
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
    # print(f"new message {msg.topic}: {str(msg.payload)}")
    plantCount=len(plants)

    if datetime.now() > plantChangeTime:
        plantChangeTime = getPlantChangeTime()
        currentPlantCount+=1
        

    currentPlantIndex = currentPlantCount % plantCount
    currentPlantName = plants[currentPlantIndex]
    currentPlantTopic = f"{plantTopics}{currentPlantName}"
    if currentPlantTopic == str(msg.topic):
        reading =int(msg.payload.decode("utf-8"))
        print (f"currentPlantCount: {currentPlantCount}, currentPlantIndex: {currentPlantIndex}, currentPlantName: {currentPlantName} ")
        
        if reading>moistureMin and reading<moistureMax:
            nextPlantTime=plantChangeTime - datetime.now()
            print(f"processing {currentPlantName}: {reading}, next plant in {nextPlantTime}")
            postToLights( currentPlantName, reading)
        


client = getMqttClient()
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
