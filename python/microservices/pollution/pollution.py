import paho.mqtt.client as mqtt
import time
from datetime import datetime,timedelta
import traceback
import psycopg2
import psycopg2.extras

import json
import os
from dotenv import load_dotenv


rootTopic ="ed33f5b2-7d08-4278-b2b0-06446de0f95d/home/tele/"
values = {
    'latitude': 'NULL',
    'longitude': 'NULL',
    'temperature': 'NULL',
    'humidity': 'NULL',
    'carbonmonoxide': 'NULL',
    'pm1': 'NULL',
    'pm2.5': 'NULL',
    'pm10': 'NULL'
}

load_dotenv()

CONNECTION = os.getenv('cockroach_db')

def getMqttClient():
    client = mqtt.Client()
    while (True):
        try:
            client.connect("broker.hivemq.com", 1883, 60)
            break
        except Exception:
            print ("error connecting, pausing")
            traceback.print_exc()
            time.sleep(5)
    return client


def on_connect(client, userdata, flags, reason_code):
    print("Connected with result code "+str(reason_code))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(f"{rootTopic}#")

def getTopic(topic):
    return f"{rootTopic}{topic}/bike"

def isTopic(expected, actual ):
    return getTopic(expected) == actual

def getTopicName(topic):
    first =topic[len(rootTopic):]
    second=first[:first.index('/')]
    return second
    # def getTopicName(topic):
    #     first = topic[:topic.index('/')]
    #     return first
    

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # print(f"new message")
    # print(msg.topic+" "+str(msg.payload))
    topicName =getTopicName(msg.topic)

    if not isTopic("location", msg.topic):
        topicName =getTopicName(msg.topic)
        values[topicName] = float(msg.payload.decode("utf-8"))
        
    else:
        print("new location")
        location = json.loads(msg.payload)
        print(location)

        # {'latitude': 51.50376511, 'longitude': -0.597308278, 'speed': 0.209999993, 'angle': 10.18000031, 'altitude': 38.59999847, 'satellites': 11}
        values['latitude'] = location['latitude']
        values['longitude'] = location['longitude']
        values['speed'] = location['speed']
        values['angle'] = location['angle']
        values['altitude'] = location['altitude']
        values['satellites'] = location['satellites']

        print(values)
        with psycopg2.connect(CONNECTION) as conn:  
            cursor = conn.cursor()
            sql =f"""insert into pollution (lat, lon, temperature, humidity, carbonmonoxide, pm1, pm25, pm10)
            values ({ values['latitude'] }, {values['longitude']}, {values['temperature']}, {values['humidity']}, {values['carbonmonoxide']}, {values['pm1']}, {values['pm2.5']}, {values['pm10']})"""
            # print (sql)
            cursor.execute(sql)
            conn.commit()

         



client = getMqttClient()
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
