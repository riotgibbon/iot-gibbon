import paho.mqtt.client as mqtt
import json
import ST7789
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

display_type = "square"
disp = ST7789.ST7789(
    height=135 if display_type == "rect" else 240,
    rotation=0 if display_type == "rect" else 90,
    port=0,
    cs=ST7789.BG_SPI_CS_FRONT,  # BG_SPI_CS_BACK or BG_SPI_CS_FRONT
    dc=9,
    backlight=19,               # 18 for back BG slot, 19 for front BG slot.
    spi_speed_hz=80 * 1000 * 1000,
    offset_left=0 if display_type == "square" else 40,
    offset_top=53 if display_type == "rect" else 0
)

disp.begin()

WIDTH = disp.width
HEIGHT = disp.height
# Clear the display to a red background.
# Can pass any tuple of red, green, blue values (from 0 to 255 each).
# Get a PIL Draw object to start drawing on the display buffer.
img = Image.new('RGB', (WIDTH, HEIGHT), color=(255, 0, 0))
draw = ImageDraw.Draw(img)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("home/tele/irgrideye/kitchen/thermal")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    tempsStr = str(msg.payload.decode("utf-8"))
    temps = json.loads(tempsStr)
    for t in temps:
        print (t)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.0.46", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()