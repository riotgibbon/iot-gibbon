import paho.mqtt.client as mqtt
import time
from threading import Thread
from multiprocessing import Process, Queue
from datetime import datetime,timedelta
# from queue import Queue 
import traceback
import requests

livingRoom={}

# The callback for when the client receives a CONNACK response from the server.

def startMQTT(q):

    def on_connect(client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("home/tele/+/livingroom/#")

    # The callback for when a PUBLISH message is received from the server.
    def on_message(client, userdata, msg):
        # print(f"new message")
        # print(f"new message {msg.topic}: {str(msg.payload)}")
        livingRoom[msg.topic]=str(msg.payload.decode("utf-8"))
        q.put(livingRoom)


    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    while (True)
        try:
            client.connect("192.168.0.63", 1883, 60)
            break
        except Exception:
            print ("error connecting, pausing")
            traceback.print_exc()
            time.sleep(5)
            
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
    client.loop_forever()

def getReadTime(seconds):
    return datetime.now() + timedelta(0,seconds)
    
def readDictionary(q):

    readTime = getReadTime(10)
    print (f"next read at {readTime}")
    # print (f"start loop")
    while(True):
        
        # print (f"reading queue")
        thisDict= q.get()      
        
        if datetime.now() > readTime:
            try:
                print (f"{len(thisDict)} items of data at {datetime.now()}: {thisDict}")
                
                #  DateTime	Aralia	Bonsai	Amaryllis	Temp	Humidity	HumidityAdj	lightReading	lightVoltage	uvIndex
                # data at 2020-11-06 19:04:11.226238: {'home/tele/temperature/livingroom/window': '28.17', 
                # 'home/tele/humidity/livingroom/window': '47.94', 'home/tele/humidityAdjusted/livingroom/window': '23.97', 
                # 'home/tele/lightReading/livingroom/window': '0', 'home/tele/lightVoltage/livingroom/window': '0', 
                # 'home/tele/uvIndex/livingroom/window': '0', 'home/tele/soilmoisture/livingroom/aralia': '833', 
                # 'home/tele/soilmoisture/livingroom/bonsai': '846', 'home/tele/soilmoisture/livingroom/amaryllis': '839'}
                delim="|"
                csv =  f"{thisDict['home/tele/soilmoisture/livingroom/aralia']}{delim}" + \
                    f"{thisDict['home/tele/soilmoisture/livingroom/bonsai']}{delim}" + \
                    f"{thisDict['home/tele/soilmoisture/livingroom/amaryllis']}{delim}" + \
                    f"{thisDict['home/tele/temperature/livingroom/window']}{delim}" + \
                    f"{thisDict['home/tele/humidity/livingroom/window']}{delim}" + \
                    f"{thisDict['home/tele/humidityAdjusted/livingroom/window']}{delim}" + \
                    f"{thisDict['home/tele/lightReading/livingroom/window']}{delim}" + \
                    f"{thisDict['home/tele/lightVoltage/livingroom/window']}{delim}" + \
                    f"{thisDict['home/tele/uvIndex/livingroom/window']}"
                print(csv)
                r = requests.post('https://maker.ifttt.com/trigger/living_room_window/with/key/d52lKnzf-xDid_NfD5tga-',data = {'value1':csv})

            except Exception:
                print ("error reading data")
                traceback.print_exc()
            finally:
                readTime = getReadTime(180)
                print (f"next read at {readTime}")


if __name__ == "__main__":
    q = Queue()
    listen= Process(target=startMQTT, args=(q,))
    listen.start()
    process = Process(target=readDictionary, args=(q,))
    process.start()
    listen.join()
    process.join()
    print("thread finished...exiting")        