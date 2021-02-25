# VEML7700

UV sensor
* UVA
* UVB
* UV Index

https://shop.pimoroni.com/products/adafruit-veml7700-lux-sensor-i2c-light-sensor


https://github.com/adafruit/Adafruit_CircuitPython_VEML7700

sudo pip3 install adafruit-circuitpython-veml7700

basic code

```
import time
import board
import busio
import adafruit_veml7700

i2c = busio.I2C(board.SCL, board.SDA)
veml7700 = adafruit_veml7700.VEML7700(i2c)

while True:
    print(f"Ambient light: {veml7700.light}, lux: {veml7700.lux}, white: {veml7700.white}")
    time.sleep(0.1)
```

mqtt- from Arduino - match


home/tele/Lux/kitchen/veml7700
home/tele/White/kitchen/veml7700
home/tele/Raw_ALS/kitchen/veml7700

