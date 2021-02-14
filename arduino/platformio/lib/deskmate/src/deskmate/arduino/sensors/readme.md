# Sensor

## MQ Unified sensors

https://components101.com/sensors/mq135-gas-sensor-for-air-quality
https://github.com/miguel5612/MQSensorsLib/


### MQ2
Methane, Butane, LPG, Smoke

https://www.pololu.com/file/0J309/MQ2.pdf

    Gas    | a      | b
    H2     | 987.99 | -2.162
    LPG    | 574.25 | -2.222
    CO     | 36974  | -3.109
    Alcohol| 3616.1 | -2.675
    Propane| 658.71 | -2.168


### MQ9
	
Carbon Monoxide, flammable gasses

http://www.haoyuelectronics.com/Attachment/MQ-9/MQ9.pdf

 GAS     | a      | b
  LPG     | 1000.5 | -2.186
  CH4     | 4269.6 | -2.648
  CO      | 599.65 | -2.244

### MQ135

Air Quality
https://www.electronicoscaldas.com/datasheet/MQ-135_Hanwei.pdf

https://www.amazon.co.uk/AZDelivery-MQ-135-Quality-Sensor-Arduino/dp/B07V9WCQL4

   Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Tolueno  | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Acetona  | 34.668 | -3.369