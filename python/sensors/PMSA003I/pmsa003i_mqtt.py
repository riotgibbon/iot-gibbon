# python3 humidity.py  --width 128 --height 128 --display sh1106 -r 2

import time
import board
import busio
from digitalio import DigitalInOut, Direction, Pull
from adafruit_pm25.i2c import PM25_I2C


reset_pin = None

import sys
import os
import math
import  logging
import paho.mqtt.client as paho

# sleepMinutes =10
# sleepSeconds = sleepMinutes * 60

sleepSeconds =5
sensor ='bme680'
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




logger.info("Connecting to " + sensor)

# Create library object, use 'slow' 100KHz frequency!
i2c = busio.I2C(board.SCL, board.SDA, frequency=100000)
# Connect to a PM2.5 sensor over I2C
pm25 = PM25_I2C(i2c, reset_pin)

logger.info("Connected to " + sensor)



broker="192.168.0.63"
port=1883
logger.info(f"Connecting to mqtt broker {broker}:{port}")
mqttClient = paho.Client("pi_" + sensor)  
mqttClient.connect(broker,port)  
logger.info(f"Connected")


def getReadTime(seconds):
    return datetime.now() + timedelta(0,seconds)



def publish(client, metric, value):
    client.publish(f"home/tele/{metric}/livingroom/desk", value)

while True:


    try:
        aqdata = pm25.read()
        # print(aqdata)

        print()
        print("Concentration Units (standard)")
        print("---------------------------------------")
        print(
            "PM 1.0: %d\tPM2.5: %d\tPM10: %d"
            % (aqdata["pm10 standard"], aqdata["pm25 standard"], aqdata["pm100 standard"])
        )
        print("Concentration Units (enviroynmental)")
        print("---------------------------------------")
        print(
            "PM 1.0: %d\tPM2.5: %d\tPM10: %d"
            % (aqdata["pm10 env"], aqdata["pm25 env"], aqdata["pm100 env"])
        )
        print("---------------------------------------")
        print("Particles > 0.3um / 0.1L air:", aqdata["particles 03um"])
        print("Particles > 0.5um / 0.1L air:", aqdata["particles 05um"])
        print("Particles > 1.0um / 0.1L air:", aqdata["particles 10um"])
        print("Particles > 2.5um / 0.1L air:", aqdata["particles 25um"])
        print("Particles > 5.0um / 0.1L air:", aqdata["particles 50um"])
        print("Particles > 10 um / 0.1L air:", aqdata["particles 100um"])
        print("---------------------------------------")

        publish(mqttClient,"pm2.5",aqdata["pm25 env"])   
        publish(mqttClient,"pm10",aqdata["pm100 env"])  

    # try:
    #     logger.info("starting bme680 read")
    #     if sensor.get_sensor_data() :
    #         bme680 = 'bme680: {0:.2f} C,{1:.2f} hPa,{2:.2f} %RH'.format(sensor.data.temperature, sensor.data.pressure, sensor.data.humidity)

    #         temperature=sensor.data.temperature
    #         pressure=sensor.data.pressure
    #         humidity=sensor.data.humidity   


    #         logger.info(bme680)
    #         # 
    #         publish(mqttClient,"temperature",temperature)   
    #         publish(mqttClient,"humidity",humidity)  
    #         publish(mqttClient,"pressure",pressure)       


    except Exception as error:
        logger.error(error.args[0])   
        time.sleep(1)
        
         
    else:
        time.sleep(sleepSeconds)

