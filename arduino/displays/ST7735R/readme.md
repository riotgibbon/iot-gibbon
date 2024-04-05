# ST7735R

https://shop.pimoroni.com/products/adafruit-1-44-color-tft-lcd-display-with-microsd-card-breakout-st7735r

https://learn.adafruit.com/adafruit-1-44-color-tft-with-micro-sd-socket

## Wiring

https://learn.adafruit.com/adafruit-1-44-color-tft-with-micro-sd-socket/wiring-and-test

#define TFT_CS     1//chip select pin for the TFT screen
#define TFT_RST    0  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     2

 

### MKR 1010

SCK - brown SCK > 9
S0 - grey - MISO > 10 (unused)
S1 - blue - MOSI > 8 
TCS - yellow > D1
RST - green > RESET (unused)
D/C - orange > D2

### ESP32

https://www.amazon.co.uk/gp/product/B08BZGC22Q
https://docs.platformio.org/en/latest/boards/espressif32/az-delivery-devkit-v4.html
https://www.az-delivery.de/en/products/esp-32-dev-kit-c-v4
https://cdn.shopify.com/s/files/1/1509/1638/files/ESP32_DevKit_C_V4_Pinout.pdf?v=1615364529

d-test

SCK - greya SCK > 6 (CLK)
S0 - grey - MISO > 19 (unused)  
S1 - purple - MOSI > 23
TCS - blue > D1
RST - green > RESET (unused)
D/C - orange > D2


ESP8266
https://simple-circuit.com/esp8266-nodemcu-st7735s-tft/

SCK - grey D5
S0 - grey - MISO 
S1 - purple - MOSI > D7
TCS - white > D3
RST - browsd > D4
D/C - orange > D2


The ST7735 display is connected to the NodeMCU board as follows:
RST pin is connected to D4 (ESP8266EX GPIO2),
CS pin is connected to D3 (ESP8266EX GPIO0),
D/C pin is connected to D2 (ESP8266EX GPIO4),
DIN pin is connected to D7 (ESP8266EX GPIO13),
CLK pin is connected to D5 (ESP8266EX GPIO14),
VCC and BL are connected to pin 3V3,
GND is connected to pin GND of the NodeMCU board.