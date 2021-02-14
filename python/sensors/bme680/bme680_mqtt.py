# python3 humidity.py  --width 128 --height 128 --display sh1106 -r 2

import time
import board
import requests
import loggingxx
import os
from datetime import datetime,timedelta
import bme680

import sys

import math
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

start_time = time.time()
curr_time = time.time()
burn_in_time = 300

burn_in_data = []
gasOK = False
try:
    # Collect gas resistance burn-in values, then use the average
    # of the last 50 values to set the upper limit for calculating
    # gas_baseline.
    print('Collecting gas resistance burn-in data for 5 mins\n')
    while curr_time - start_time < burn_in_time:
        curr_time = time.time()
        if sensor.get_sensor_data() and sensor.data.heat_stable:
            gas = sensor.data.gas_resistance
            burn_in_data.append(gas)
            print('Gas: {0} Ohms'.format(gas))
            time.sleep(1)

    gas_baseline = sum(burn_in_data[-50:]) / 50.0

    # Set the humidity baseline to 40%, an optimal indoor humidity.
    hum_baseline = 40.0

    # This sets the balance between humidity and gas reading in the
    # calculation of air_quality_score (25:75, humidity:gas)
    hum_weighting = 0.25

    print('Gas baseline: {0} Ohms, humidity baseline: {1:.2f} %RH\n'.format(
        gas_baseline,
        hum_baseline))
    gasOK=True
except Exception as error:
    logger.error(error.args[0])    
    gasOK=False


broker="192.168.0.46"
port=1883
logger.info(f"Connecting to mqtt broker {broker}:{port}")
mqttClient = paho.Client("bme680")  
mqttClient.connect(broker,port)  
logger.info(f"Connected")
bme680Read = False

def getReadTime(seconds):
    return datetime.now() + timedelta(0,seconds)


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
            publish(mqttClient,"temperature",temperature)   
            publish(mqttClient,"humidity",humidity)  
            publish(mqttClient,"pressure",pressure)  

            if sensor.get_sensor_data() and sensor.data.heat_stable and gasOK:
                gas = sensor.data.gas_resistance
                gas_offset = gas_baseline - gas

                hum = sensor.data.humidity
                hum_offset = hum - hum_baseline

                # Calculate hum_score as the distance from the hum_baseline.
                if hum_offset > 0:
                    hum_score = (100 - hum_baseline - hum_offset)
                    hum_score /= (100 - hum_baseline)
                    hum_score *= (hum_weighting * 100)

                else:
                    hum_score = (hum_baseline + hum_offset)
                    hum_score /= hum_baseline
                    hum_score *= (hum_weighting * 100)

                # Calculate gas_score as the distance from the gas_baseline.
                if gas_offset > 0:
                    gas_score = (gas / gas_baseline)
                    gas_score *= (100 - (hum_weighting * 100))

                else:
                    gas_score = 100 - (hum_weighting * 100)

                # Calculate air_quality_score.
                air_quality_score = hum_score + gas_score

                print('Gas: {0:.2f} Ohms,humidity: {1:.2f} %RH,air quality: {2:.2f}'.format(
                    gas,
                    hum,
                    air_quality_score))  
                
                publish(mqttClient,"gas",gas)   
                publish(mqttClient,"air_quality_score",air_quality_score)    

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

