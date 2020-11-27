import requests
import json
import paho.mqtt.client as paho
from datetime import datetime,timedelta
import os
import logging
import time

# url = "https://rapidapi.p.rapidapi.com/weather"
location = "Slough"
country= "uk"

querystring = {"q":f"{location},{country}","units":"metric"}

headers = {
    'x-rapidapi-key': "b15a25b44cmsh2606123013f2f9dp1a6645jsn39377a256bc2",
    'x-rapidapi-host': "community-open-weather-map.p.rapidapi.com"
    }

source = "openweather"

key='b5807e59dfa1bcb69304c35d5ef03dc4'
url=f'http://api.openweathermap.org/data/2.5/weather'#?q={location}&appid={key}'
querystring = {"q":f"{location}","appid":key,"units":"metric"}

logroot = 'logs'
os.makedirs (logroot,exist_ok=True)
logger = logging.getLogger(source)
logname = datetime.now().strftime(f"{source}_%Y-%m-%d.log")
logpath = os.path.join(logroot,logname)
hdlr = logging.FileHandler(logpath)
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
logging.getLogger().addHandler(logging.StreamHandler())
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)



broker="192.168.0.63"
port=1883
logger.info(f"Connecting to mqtt broker {broker}:{port}")
mqttClient = paho.Client(source)  
mqttClient.connect(broker,port)  
logger.info(f"Connected")

sleepSeconds = 60

def publish(client, metric, value):
    topic =f"home/tele/{metric}/{location}/{source}"
    print (f"publishing: {topic}:{value}")
    client.publish(topic, value)

while True:
    try:

        response = requests.request("GET", url, headers=headers, params=querystring)


        logger.info(response.text)

        weather = json.loads(response.text)

        readings = weather['main']

        temperature=readings['temp']
        pressure=readings['pressure']
        humidity=readings['humidity']   


        publish(mqttClient,"temperature",temperature)   
        publish(mqttClient,"humidity",humidity)  
        publish(mqttClient,"pressure",pressure)    

    
    except Exception as error:
        logger.error(error.args[0])   

    time.sleep(sleepSeconds)
