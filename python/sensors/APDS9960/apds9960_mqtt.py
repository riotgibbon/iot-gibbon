import board
import busio
import digitalio
from adafruit_apds9960.apds9960 import APDS9960
from adafruit_apds9960 import colorutility
import time
import paho.mqtt.client as paho
import os
import logging
from datetime import datetime,timedelta
import requests

sleepSeconds =5
sensor ='apds9960'
logroot = 'logs'
os.makedirs (logroot,exist_ok=True)
logger = logging.getLogger(sensor)
logname = datetime.now().strftime(f"{sensor}_%Y-%m-%d.log")
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
mqttClient = paho.Client(sensor)  
mqttClient.connect(broker,port)  
logger.info(f"Connected")


i2c = busio.I2C(board.SCL, board.SDA)
int_pin = digitalio.DigitalInOut(board.D5)
apds = APDS9960(i2c, interrupt_pin=int_pin)

# apds.enable_proximity = True
# apds.proximity_interrupt_threshold = (0, 175)
# apds.enable_proximity_interrupt = True
apds.enable_proximity = True

apds.enable_gesture = True
apds.enable_color = True


def publish(client, metric, value):
    topic =f"home/tele/{metric}/livingroom/desk"
    print (f"publishing: {topic}:{value}")
    client.publish(topic, value)

def getReadTime(seconds):
    return datetime.now() + timedelta(0,seconds)

def postToIFTT(event):    
    r = requests.get(f'https://maker.ifttt.com/trigger/{event}/with/key/d52lKnzf-xDid_NfD5tga-')


while not apds.color_data_ready:
    time.sleep(0.005)

readTime = getReadTime(0)

while True:
        try:
            if datetime.now() > readTime:
                while not apds.color_data_ready:
                    time.sleep(0.005)
                r, g, b, c = apds.color_data
                print(f"red: {r}, green: {g}, blue: {b}, clear: {c}")

                publish(mqttClient,"red",r)   
                publish(mqttClient,"green",g)   
                publish(mqttClient,"blue",b)   
                publish(mqttClient,"clear",c)   

                colourTemp=colorutility.calculate_color_temperature(r, g, b)
                lightLux=colorutility.calculate_lux(r, g, b)
                publish(mqttClient,"colourTemp",colourTemp)   
                publish(mqttClient,"lightLux",lightLux)   
                
                readTime = getReadTime(sleepSeconds)

            gesture = apds.gesture()

            if gesture == 0x01:
                print("up")
                publish(mqttClient,"gesture","up") 
                postToIFTT('desk_up') 

            elif gesture == 0x02:
                print("down")
                publish(mqttClient,"gesture","down")  
                postToIFTT('desk_down')

            elif gesture == 0x03:
                print("left")
                publish(mqttClient,"gesture","left")  
                postToIFTT('desk_left')

            elif gesture == 0x04:
                print("right")
                publish(mqttClient,"gesture","right")  
                postToIFTT('desk_right')



            



        except Exception as error:
            logger.error(error.args[0])   


        