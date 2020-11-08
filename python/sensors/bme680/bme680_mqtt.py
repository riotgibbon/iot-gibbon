# python3 humidity.py  --width 128 --height 128 --display sh1106 -r 2

import time
import board
import requests
import logging
import os
from datetime import datetime
import bme680

import sys

import math
import paho.mqtt.client as paho

# sleepMinutes =10
# sleepSeconds = sleepMinutes * 60

sleepSeconds =5

logroot = 'logs'
os.makedirs (logroot,exist_ok=True)
logger = logging.getLogger('bme680')
logname = datetime.now().strftime("bme680_%Y-%m-%d.log")
logpath = os.path.join(logroot,logname)
hdlr = logging.FileHandler(logpath)
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
logging.getLogger().addHandler(logging.StreamHandler())
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)




logger.info("Connecting to bme680")

try:
    sensor = bme680.BME680(bme680.I2C_ADDR_PRIMARY)
except IOError:
    sensor = bme680.BME680(bme680.I2C_ADDR_SECONDARY)

logger.info("Connected to bme680")


logger.info('Calibration data:')
for name in dir(sensor.calibration_data):

    if not name.startswith('_'):
        value = getattr(sensor.calibration_data, name)

        if isinstance(value, int):
            logger.info('{}: {}'.format(name, value))

# These oversampling settings can be tweaked to
# change the balance between accuracy and noise in
# the data.

sensor.set_humidity_oversample(bme680.OS_2X)
sensor.set_pressure_oversample(bme680.OS_4X)
sensor.set_temperature_oversample(bme680.OS_8X)
sensor.set_filter(bme680.FILTER_SIZE_3)
sensor.set_gas_status(bme680.ENABLE_GAS_MEAS)

logger.info('\n\nInitial reading:')
for name in dir(sensor.data):
    value = getattr(sensor.data, name)

    if not name.startswith('_'):
        logger.info('{}: {}'.format(name, value))

sensor.set_gas_heater_temperature(320)
sensor.set_gas_heater_duration(150)
sensor.select_gas_heater_profile(0)


broker="192.168.0.63"
port=1883
logger.info(f"Connecting to mqtt broker {broker}:{port}")
mqttClient = paho.Client("bme680")  
mqttClient.connect(broker,port)  
logger.info(f"Connected")
bme680Read = False

def getReadTime(seconds):
    return datetime.now() + timedelta(0,seconds)

mqttWriteTime = getReadTime(10)
print (f"next read at {readTime}")
ifftWriteTime = getReadTime(10)

def publish(client, metric, value):
    client.publish(f"home/tele/{metric}/livingroom/desk", value)

while True:
    try:
        logger.info("starting bme680 read")
        if sensor.get_sensor_data() :
            bme680 = 'bme680: {0:.2f} C,{1:.2f} hPa,{2:.2f} %RH'.format(sensor.data.temperature, sensor.data.pressure, sensor.data.humidity)

            temperature=sensor.data.temperature
            pressure=sensor.data.pressure
            humidity=sensor.data.humidity


            logger.info(bme680)
            # 
            publish("temperature",temperature)   
            publish("humidity",humidity)  
            publish("humidity",pressure)       

            if datetime.now() > ifftWriteTime:
                try:
                    r = requests.post('https://maker.ifttt.com/trigger/bme680/with/key/d52lKnzf-xDid_NfD5tga-',data = {'value1':sensor.data.temperature, 'value2': sensor.data.humidity, 'value3': sensor.data.pressure})
                    ifftWriteTime = getReadTime(300)
                    logger.info(f"sent bme680 to IFTTT: {r}")
                except Exception:
                    print ("error writing to IFTTT")
                    traceback.print_exc()    


    except Exception as error:
        logger.error(error.args[0])   
        time.sleep(1)
         
    else:
        time.sleep(sleepSeconds)

