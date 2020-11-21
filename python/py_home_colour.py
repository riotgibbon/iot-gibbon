import paho.mqtt.client as mqtt
import json



def getMqttClient():
    client = mqtt.Client()
    while (True):
        try:
            client.connect("192.168.0.63", 1883, 60)
            break
        except Exception:
            print ("error connecting, pausing")
            traceback.print_exc()
            time.sleep(5)
    return client

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(f"home/cmd/hue/tv")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # print(f"new message {msg.topic}: {str(msg.payload)}")
    body = json.loads(msg.payload)
    # print(body)
    plant = body['plant']['name']
    print(f"plant: {plant}")
    hue =  (body['hue'])
    print (hue)
    state =hue[0]
    print(f"state: {state}")
    xy = state['xy']
    bri =  body['hue']['state']['bri']
    print(f"plant: {plant}, xy:{xy}, bri:{bri}")

        


client = getMqttClient()
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
