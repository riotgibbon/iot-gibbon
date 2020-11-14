#!/usr/bin/env python3
import time
import ioexpander as io


ioe = io.IOE(i2c_addr=0x18)

ioe.set_adc_vref(3.3)  # Input voltage of IO Expander, this is 3.3 on Breakout Garden
ioe.set_mode(14, io.ADC)

last_adc = 0.00
present=0
while True:
    adc = ioe.input(14)
    adc = round(adc, 4)
    present=0
    if adc != last_adc:
        if adc>0.5 and adc<0.9:
            present=1
            
        print(f"present: {present}, {adc:.4f}v")
        last_adc = adc

    time.sleep(1)