import board
import busio
import digitalio
from adafruit_apds9960.apds9960 import APDS9960
import time

i2c = busio.I2C(board.SCL, board.SDA)
int_pin = digitalio.DigitalInOut(board.D5)
apds = APDS9960(i2c, interrupt_pin=int_pin)

apds.enable_proximity = True
apds.proximity_interrupt_threshold = (0, 175)
apds.enable_proximity_interrupt = True

apds.enable_color = True

while not apds.color_data_ready:
    time.sleep(0.005)

while True:
        print(f"proximity: {apds.proximity}")
        apds.clear_interrupt()

        r, g, b, c = apds.color_data
        print("r: {}, g: {}, b: {}, c: {}".format(r, g, b, c))

       