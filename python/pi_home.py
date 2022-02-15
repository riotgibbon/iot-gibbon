# python3 humidity.py  --width 128 --height 128 --display sh1106 -r 2

import time
import board
import requests
import logging
import os
from datetime import datetime, timedelta
import bme680
from luma.core.legacy import show_message
import sys
from luma.core import cmdline, error
from luma.core.legacy.font import proportional, SINCLAIR_FONT
from luma.core.render import canvas
import math
from influxdb import InfluxDBClient

def getInfluxClient(host='192.168.0.46', port=8086):
    user=''
    password=''
    dbname = 'home'
    client = InfluxDBClient(host, port, user, password, dbname)
    print (f"Got influx @{host}")
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

def strfdelta(tdelta, fmt):
    d = {"days": tdelta.days}
    d["hours"], rem = divmod(tdelta.seconds, 3600)
    d["minutes"], d["seconds"] = divmod(rem, 60)
    return fmt.format(**d)

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

client = getInfluxClient(host='192.168.0.46')
query = "SELECT MEAN(value) FROM mqtt_consumer   WHERE time > now() - 30s group by * ;"

def getTopicValue(result, topic):
    try:
        return list(result.get_points(measurement='mqtt_consumer', tags={'topic': topic}))[0]['mean']
    except Exception as error:
        return 0

was_present = False
today_last_time = "Unknown"
presentFrom = datetime.now()


margin = 4

cx = 30
cy = min(device.height, 64) / 2

left = cx - cy
right = cx + cy

while True:
    
    now = datetime.now()
    today_date = now.strftime("%d %b %y")
    today_time = now.strftime("%H:%M:%S")
    if today_time != today_last_time:
        result = client.query(query)
        # print(f"got data {result}")
        temperature =getTopicValue(result,'home/tele/temperature/livingroom/desk')
        humidity=getTopicValue(result,'home/tele/humidity/livingroom/desk')
        presentMean=getTopicValue(result,'home/tele/present/livingroom/desk')
        is_present =presentMean>0

        proximityVoltage =getTopicValue(result,'home/tele/proximityVoltage/livingroom/desk')
        if is_present :
            print(f"present: {present}")
            if not was_present:
                logging.info("Switching light on")
                r = requests.get('https://maker.ifttt.com/trigger/Light_Desk_On/with/key/d52lKnzf-xDid_NfD5tga-')
                was_present =True
                presentFrom = datetime.now()

            today_last_time = today_time
            presentFor =datetime.now() - presentFrom 
            presentForHMS = strfdelta(presentFor, "{hours}:{minutes}:{seconds}")

            with canvas(device) as draw:
                now = datetime.now()
                today_date = now.strftime("%d %b %y")



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
                draw.text((2 * (cx + margin), cy+24), f"{str(presentForHMS)} ", fill="yellow")
                draw.text((2 * (cx + margin), cy+32), f"{str(proximityVoltage)} ", fill="yellow")
                draw.text((2 * (cx + margin), cy+40), f"{str(presentMean)} ", fill="yellow")
        else:
            with canvas(device) as draw:
                device.clear()
            #     draw.text((2 * (cx + margin), cy+32), f"{str(proximityVoltage)} ", fill="yellow")
            #     draw.text((2 * (cx + margin), cy+40), f"{str(presentMean)} ", fill="yellow")
                
            if was_present:
                logging.info("Switching light off")
                r = requests.get('https://maker.ifttt.com/trigger/Light_Desk_Off/with/key/d52lKnzf-xDid_NfD5tga-')
                was_present =False
    time.sleep(0.5)


