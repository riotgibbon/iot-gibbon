import paho.mqtt.client as mqtt
import json
import ST7789


from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

disp = ST7789.ST7789(
    port=0,
    cs=ST7789.BG_SPI_CS_FRONT,  # BG_SPI_CS_BACK or BG_SPI_CS_FRONT
    dc=9,
    backlight=19,               # 18 for back BG slot, 19 for front BG slot.
    rotation=90,
    spi_speed_hz=80 * 1000 * 1000
)

# Initialize display.
disp.begin()

WIDTH = disp.width
HEIGHT = disp.height


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
    print(f"body: {body}")
    plant = body['plant']['name']
    print(f"plant: {plant}")
    hue =  (body['hue'])
    print (f"hue: {hue}")

    xy = hue['xy']
    x= xy[0]
    y=xy[1]
    bri =  hue['bri']

    print(f"plant: {plant}, xy:{xy}, bri:{bri}")


    rgb=hue['rgb']
    r,g,b = rgb[0], rgb[1], rgb[2]

    print(f"r,g,b : {r},{g},{b}")

    img = Image.new('RGB', (WIDTH, HEIGHT), color=(rgb))

    draw = ImageDraw.Draw(img)
    draw.rectangle((10, 10, WIDTH - 10, HEIGHT - 10), outline=(r,g,b), fill=(r,g,b))

    disp.display(img)
        


client = getMqttClient()
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
