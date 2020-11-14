#!/usr/bin/env python3
import time
import ioexpander as io
import paho.mqtt.client as paho
import os
import logging
from datetime import datetime,timedelta

ioe = io.IOE(i2c_addr=0x18)

ioe.set_adc_vref(3.3)  # Input voltage of IO Expander, this is 3.3 on Breakout Garden
ioe.set_mode(14, io.ADC)

sensor ='gp2y0a21yk0f'
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

def publish(client, metric, value):
    topic =f"home/tele/{metric}/livingroom/desk"
    print (f"publishing: {topic}:{value}")
    client.publish(topic, value)


last_adc = 0.00
present=0
while True:
    adc = ioe.input(14)
    adc = round(adc, 4)
    present=0
    if adc != last_adc:
        if adc>0.2 and adc<1:
            present=1
        publish(mqttClient,"present",present)  
        publish(mqttClient,"proximityVoltage",adc)     
        print(f"present: {present}, {adc:.4f}v")
        last_adc = adc

    time.sleep(1)