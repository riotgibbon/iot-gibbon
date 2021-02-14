import paho.mqtt.client as mqtt
import time
from threading import Thread
from multiprocessing import Process, Queue
from datetime import datetime,timedelta
# from queue import Queue 


livingRoom={}

# The callback for when the client receives a CONNACK response from the server.

def startMQTT(q):

    def on_connect(client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("#")

    # The callback for when a PUBLISH message is received from the server.
    def on_message(client, userdata, msg):
        # print(f"new message")
        #print(f"new message {msg.topic}: {str(msg.payload)}")
        livingRoom[msg.topic]=str(msg.payload.decode("utf-8"))
        q.put(livingRoom)


    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect("192.168.0.46", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
    client.loop_forever()

def getReadTime():
    return datetime.now() + timedelta(0,10)
    
def readDictionary(q):

    readTime = getReadTime()
    print (f"next read at {readTime}")
    # print (f"start loop")
    while(True):
        
        # print (f"reading queue")
        thisDict= q.get()      
        
        if datetime.now() > readTime:
            print (f"data at {datetime.now()}: {thisDict}")
        

            # print(f"dictionary contents")

            # for x, y in thisDict.items():
            #     print(x, y)
            readTime = getReadTime()
            print (f"next read at {readTime}")


if __name__ == "__main__":
    q = Queue()
    client= Process(target=startMQTT, args=(q,))
    client.start()
    read = Process(target=readDictionary, args=(q,))
    read.start()
    client.join()
    read.join()
    print("thread finished...exiting")        