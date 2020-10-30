import time
import board
import adafruit_dht
import requests
import logging
import os
from datetime import datetime
import bme680


# Initial the dht device, with data pin connected to:

sleepMinutes =10
sleepSeconds = sleepMinutes * 60


logroot = 'logs'
os.makedirs (logroot,exist_ok=True)
logger = logging.getLogger('DHT22')
logname = datetime.now().strftime("DHT22_%Y-%m-%d.log")
logpath = os.path.join(logroot,logname)
hdlr = logging.FileHandler(logpath)
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
logging.getLogger().addHandler(logging.StreamHandler())
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)

#logger.info(f"Connecting to pin {board.D4}") 
#dhtDevice = adafruit_dht.DHT22(board.D4)
#logger.info(f"Connected") 

dhtPin =board.D5
logger.info(f"Connecting to DHT22 pin {dhtPin}") 
dhtDevice = adafruit_dht.DHT22(dhtPin, use_pulseio=False)
logger.info(f"Connected") 

while True:



    try:    
        temp = dhtDevice.temperature
        humidity = dhtDevice.humidity
        logger.info(f"DHT22: temp{temp}, humidity: {humidity}") 
        r = requests.post('https://maker.ifttt.com/trigger/DHT22/with/key/d52lKnzf-xDid_NfD5tga-',data = {'value1': temp, 'value2': humidity})
        logger.info(f"sent DHT22 to IFTTT: {r}")  
        time.sleep(sleepSeconds)
    except Exception as error:
# Errors happen fairly often, DHT's are hard to read, just keep going
        logger.error(error.args[0])
        time.sleep(2.0)
    logger.info(f"finished waiting")
