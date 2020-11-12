# python3 humidity.py  --width 128 --height 128 --display sh1106 -r 2

import time
import board
import requests
import logging
import os
from datetime import datetime
import bme680
from luma.core.legacy import show_message
import sys
from luma.core import cmdline, error
from luma.core.legacy.font import proportional, SINCLAIR_FONT
from luma.core.render import canvas
import math
from influxdb import InfluxDBClient

def getInfluxClient(host='localhost', port=8086):
    user=''
    password=''
    dbname = 'home'
    client = InfluxDBClient(host, port, user, password, dbname)
    return client


def posn(angle, arm_length):
    dx = int(math.cos(math.radians(angle)) * arm_length)
    dy = int(math.sin(math.radians(angle)) * arm_length)
    return (dx, dy)

def display_settings(device, args):
    """
    Display a short summary of the settings.
    :rtype: str
    """
    iface = ''
    display_types = cmdline.get_display_types()
    if args.display not in display_types['emulator']:
        iface = 'Interface: {}\n'.format(args.interface)

    lib_name = cmdline.get_library_for_display_type(args.display)
    if lib_name is not None:
        lib_version = cmdline.get_library_version(lib_name)
    else:
        lib_name = lib_version = 'unknown'

    #--width 128 --height 128 --display sh1106 -r 2

    import luma.core
    version = 'luma.{} {} (luma.core {})'.format(
        lib_name, lib_version, luma.core.__version__)

    return 'Version: {}\nDisplay: {}\n{}Dimensions: {} x {}\n{}'.format(
        version, args.display, iface, device.width, device.height, '-' * 60)

def clockReadings(temperature, humidity, pressure):
    today_last_time = "Unknown"

    now = datetime.now()
    today_date = now.strftime("%d %b %y")
    today_time = now.strftime("%H:%M:%S")
    if today_time != today_last_time:
        today_last_time = today_time
        with canvas(device) as draw:
            now = datetime.now()
            today_date = now.strftime("%d %b %y")

            margin = 4

            cx = 30
            cy = min(device.height, 64) / 2

            left = cx - cy
            right = cx + cy

            hrs_angle = 270 + (30 * (now.hour + (now.minute / 60.0)))
            hrs = posn(hrs_angle, cy - margin - 7)

            min_angle = 270 + (6 * now.minute)
            mins = posn(min_angle, cy - margin - 2)

            sec_angle = 270 + (6 * now.second)
            secs = posn(sec_angle, cy - margin - 2)

            draw.ellipse((left + margin, margin, right - margin, min(device.height, 64) - margin), outline="white")
            draw.line((cx, cy, cx + hrs[0], cy + hrs[1]), fill="white")
            draw.line((cx, cy, cx + mins[0], cy + mins[1]), fill="white")
            draw.line((cx, cy, cx + secs[0], cy + secs[1]), fill="red")
            draw.ellipse((cx - 2, cy - 2, cx + 2, cy + 2), fill="white", outline="white")
            draw.text((2 * (cx + margin), cy - 8), today_date, fill="yellow")
            draw.text((2 * (cx + margin), cy), today_time, fill="yellow")
            # draw.text((2 * (cx + margin), cy+8), f"{str(temperature)} C", fill="yellow")
            # draw.text((2 * (cx + margin), cy+16), f"{str(humidity)} %", fill="yellow")

def get_device(actual_args=None):
    """
    Create device from command-line arguments and return it.
    """
    if actual_args is None:
        actual_args = sys.argv[1:]
    parser = cmdline.create_parser(description='luma.examples arguments')
    args = parser.parse_args(actual_args)

    if args.config:
        # load config from file
        config = cmdline.load_config(args.config)
        args = parser.parse_args(config + actual_args)

    args
    # create device
    try:
        args.width=128
        args.height=128
        args.display='sh1106'
        args.rotate=2

        device = cmdline.create_device(args)
        print(display_settings(device, args))
        return device

    except error.Error as e:
        parser.error(e)
        return None

# Initial the dht device, with data pin connected to:

sleepMinutes =10
sleepSeconds = sleepMinutes * 60


logroot = 'logs'
os.makedirs (logroot,exist_ok=True)
logger = logging.getLogger('pi_home')
logname = datetime.now().strftime("pi_home_%Y-%m-%d.log")
logpath = os.path.join(logroot,logname)
hdlr = logging.FileHandler(logpath)
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
logging.getLogger().addHandler(logging.StreamHandler())
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)



device = get_device()

client = getInfluxClient()
query = "select last(value),* from mqtt_consumer group by *;"

def getTopicValue(result, topic):
    return list(result.get_points(measurement='mqtt_consumer', tags={'topic': topic}))[0]['value']

was_present = False
today_last_time = "Unknown"
while True:
    
    now = datetime.now()
    today_date = now.strftime("%d %b %y")
    today_time = now.strftime("%H:%M:%S")
    if today_time != today_last_time:
        result = client.query(query)
        temperature =getTopicValue(result,'home/tele/temperature/livingroom/desk')
        humidity=getTopicValue(result,'home/tele/humidity/livingroom/desk')
        present =getTopicValue(result,'home/tele/proximity/livingroom/desk')>5

        if present :
            if not was_present:
                r = requests.get('https://maker.ifttt.com/trigger/Light_Desk_On/with/key/d52lKnzf-xDid_NfD5tga-')
                was_present =True
            
            today_last_time = today_time


            with canvas(device) as draw:
                now = datetime.now()
                today_date = now.strftime("%d %b %y")

                margin = 4

                cx = 30
                cy = min(device.height, 64) / 2

                left = cx - cy
                right = cx + cy

                hrs_angle = 270 + (30 * (now.hour + (now.minute / 60.0)))
                hrs = posn(hrs_angle, cy - margin - 7)

                min_angle = 270 + (6 * now.minute)
                mins = posn(min_angle, cy - margin - 2)

                sec_angle = 270 + (6 * now.second)
                secs = posn(sec_angle, cy - margin - 2)

                draw.ellipse((left + margin, margin, right - margin, min(device.height, 64) - margin), outline="white")
                draw.line((cx, cy, cx + hrs[0], cy + hrs[1]), fill="white")
                draw.line((cx, cy, cx + mins[0], cy + mins[1]), fill="white")
                draw.line((cx, cy, cx + secs[0], cy + secs[1]), fill="red")
                draw.ellipse((cx - 2, cy - 2, cx + 2, cy + 2), fill="white", outline="white")
                draw.text((2 * (cx + margin), cy - 8), today_date, fill="yellow")
                draw.text((2 * (cx + margin), cy), today_time, fill="yellow")
                draw.text((2 * (cx + margin), cy+8), f"{str(temperature)} C", fill="yellow")
                draw.text((2 * (cx + margin), cy+16), f"{str(humidity)} %", fill="yellow")
        else:
            device.clear()
             if was_present:
                r = requests.get('https://maker.ifttt.com/trigger/Light_Desk_Off/with/key/d52lKnzf-xDid_NfD5tga-')
                was_present =False
    time.sleep(0.1)



    # temperature=""
    # pressure=""
    # humidity=""
    # clockReadings(temperature, humidity, pressure)
    # time.sleep(0.1)

    # try:
    #     logger.info("starting bme680 read")
    #     if bme680Read == False and sensor.get_sensor_data() :
    #                 bme680 = 'bme680: {0:.2f} C,{1:.2f} hPa,{2:.2f} %RH'.format(sensor.data.temperature, sensor.data.pressure, sensor.data.humidity)

    #                 temperature=sensor.data.temperature
    #                 pressure=sensor.data.pressure
    #                 humidity=sensor.data.humidity
                    
    #                 logger.info(bme680)
    #                 # show_message(device, output, fill="white", font=proportional(SINCLAIR_FONT))
    #                 # show_message(device, bme680, fill="white", font=SINCLAIR_FONT)
    #                 clockReadings(temperature, humidity, pressure)
    #                 r = requests.post('https://maker.ifttt.com/trigger/bme680/with/key/d52lKnzf-xDid_NfD5tga-',data = {'value1':sensor.data.temperature, 'value2': sensor.data.humidity, 'value3': sensor.data.pressure})
    #                 logger.info(f"sent bme680 to IFTTT: {r}")
    #                 bme680Read = True
        # logger.info("starting dht22Read read")            
#         if dht22Read == False:
#             try:    
#                 temp = dhtDevice.temperature
#                 humidity = dhtDevice.humidity
#                 logger.info(f"DHT22: temp{temp}, humidity: {humidity}") 
#                 r = requests.post('https://maker.ifttt.com/trigger/DHT22/with/key/d52lKnzf-xDid_NfD5tga-',data = {'value1': temp, 'value2': humidity})
#                 logger.info(f"sent DHT22 to IFTTT: {r}")  
#                 dht22Read = True
#             except Exception as error:
# # Errors happen fairly often, DHT's are hard to read, just keep going
# #                 logger.error(error.args[0])
#     except Exception as error:
#     # Errors happen fairly often, DHT's are hard to read, just keep going
#         logger.error(error.args[0])   
        
#     if dht22Read and bme680Read:
#         bme680Read = False
#         # dht22Read = False
#         time.sleep(sleepSeconds)
#     else:
#         time.sleep(1)

