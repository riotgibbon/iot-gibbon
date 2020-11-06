# MQTT

## Paho MQTT


https://pypi.org/project/paho-mqtt/
pip3 install paho-mqtt


code

git clone https://github.com/eclipse/paho.mqtt.python
cd paho.mqtt.python
python setup.py install


## Core pattern for aggregating readings

Listen on one process, collect latest topic values in a dictionary, process on second process, pass the incoming values to from `listen` to `process` using a queue.

This aggregation is only required if multiple values are required at the same time to be processed together (eg all the window or temperature readings)

This code can probably be abstracted, passing in parameters for topic pattern, host and processing function

### Initialise

```
if __name__ == "__main__":
    q = Queue()
    listen= Process(target=startMQTT, args=(q,))
    listen.start()
    process = Process(target=readDictionary, args=(q,))
    process.start()
    listen.join()
    process.join()
    print("thread finished...exiting")   
```

### Listen

Define the host and the topic pattern to subscribe to. When new messages come in, they are added to the dictionary and the whole dictionary put on the queue

```
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

    client.connect("192.168.0.63", 1883, 60)
```

### Process

constantly loop and collect items from the queue. Create a `readTime` datetime to define when actions should take place, and constantly move it on

```
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
                # do something with values from thisDict[topicName]


            except Exception:
                print ("error reading data")
                traceback.print_exc()
            finally:
                readTime = getReadTime(180)
                print (f"next read at {readTime}")
```