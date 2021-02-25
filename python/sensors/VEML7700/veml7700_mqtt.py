# python3 humidity.py  --width 128 --height 128 --display sh1106 -r 2

import time
import board

import logging
import os
from datetime import datetime,timedelta
import busio
import adafruit_veml7700

import sys

import math
import paho.mqtt.client as paho

# sleepMinutes =10
# sleepSeconds = sleepMinutes * 60


location="livingroom/desk"
sleepSeconds =5
sensor ='veml7700'

locationSensor = f"{location}/{sensor}"

logroot = 'logs'
os.makedirs (logroot,exist_ok=True)
logger = logging.getLogger(locationSensor)
logname = datetime.now().strftime(f"{locationSensor}_%Y-%m-%d.log")
logpath = os.path.join(logroot,logname)
hdlr = logging.FileHandler(logpath)
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
logging.getLogger().addHandler(logging.StreamHandler())
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)




logger.info(f"Connecting to {locationSensor}")



broker="192.168.0.46"
port=1883
logger.info(f"Connecting to mqtt broker {broker}:{port}")
mqttClient = paho.Client(locationSensor)  
mqttClient.connect(broker,port)  
logger.info(f"Connected")

i2c = busio.I2C(board.SCL, board.SDA)
veml7700 = adafruit_veml7700.VEML7700(i2c)

def getReadTime(seconds):
    return datetime.now() + timedelta(0,seconds)



def publish(client, metric, value):
    client.publish(f"home/tele/{metric}/{locationSensor}", value)

while True:
    try:
        logger.info(f"starting {locationSensor} read")

        logger.info(f"Ambient light: {veml7700.light}, lux: {veml7700.lux}, white: {veml7700.white}")
        
        publish(mqttClient,"Raw_ALS",veml7700.light)   
        publish(mqttClient,"White",veml7700.white)  
        publish(mqttClient,"Lux",veml7700.lux)  


    except Exception as error:
        logger.error(error.args[0])   
        time.sleep(1)
         
    else:
        time.sleep(sleepSeconds)

