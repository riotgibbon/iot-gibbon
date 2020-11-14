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


sensor ='apds9960_gesture'
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


def publish(client, metric, value):
    topic =f"home/tele/{metric}/livingroom/desk"
    print (f"publishing: {topic}:{value}")
    client.publish(topic, value)

while True:
        try:

            gesture = apds.gesture()

            if gesture == 0x01:
                print("up")
                publish(mqttClient,"gesture","up")   
            elif gesture == 0x02:
                print("down")
                publish(mqttClient,"gesture","down")  
            elif gesture == 0x03:
                print("left")
                publish(mqttClient,"gesture","left")  
            elif gesture == 0x04:
                print("right")
                publish(mqttClient,"gesture","right")  


        except Exception as error:
            logger.error(error.args[0])   

