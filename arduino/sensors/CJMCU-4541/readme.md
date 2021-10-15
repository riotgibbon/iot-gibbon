# CJMCU-4541 / MICS-4514

is a MICS-4514

https://www.banggood.com/CJMCU-4541-MICS-4514-Carbon-Monoxide-Nitrogen-Oxygen-Sensor-CONO2H2NH3CH4-p-1183263.html



https://forum.arduino.cc/index.php?topic=517952.0

https://github.com/ShawnHymel/MICS-4514_CO_and_NOx_Sensor_Breakout

#define PRE_PIN          8 # D1
#define VNOX_PIN         A0  # =VP / 36
#define VRED_PIN         A3  #  VN / 39



might be a https://wiki.seeedstudio.com/Grove-Multichannel_Gas_Sensor/

https://bochovj.wordpress.com/2019/08/15/airsense-3rd-data-collection-and-some-conclusions/
`The CJMCU 4541 NOx channel is equivalent to the Mics 2714.` =

https://www.instructables.com/The-CANARY-Arduino-Based-NO2-Sensor-and-Mapper/



https://www.instructables.com/Air-Pollution-Detector/


https://www.cdiweb.com/datasheets/e2v/mics-2714.pdf




https://www.mercersec.org/sites/default/files/webform/fair_application/180/NOx%20monitoring%20device.pdf


MICS 2714:

//Calculate PPM value
float calculatePPM(int analogVal) {
float mvoltage = (analogVal * (5.0 / 1023.0)) / 1000;
float rawSensorVal = ((1.8 * 2) / mvoltage) - 2;
float ppm = pow(10, (log10(rawSensorVal) - 2.176) / (-1.737));
return (ppm);
}



https://github.com/panStamp/swap/blob/master/examples/respira/sensor.ino


## Expected amouns


https://uk-air.defra.gov.uk/library/assets/documents/reports/aqeg/nitrogen_dioxide_in_the_UK-summary.pdf
EU First Daughter Directive (99/30/EC)

200 µg m-3 with no more than 40 µg m-3 2010
 18 exceedences per year

https://www.gov.uk/government/statistics/air-quality-statistics/ntrogen-dioxide#average-hours-spent-in-moderate-or-higher-no2-pollution
 This metric measures the annual trend in the number of hours on average that concentrations are recorded at levels that may have impacts on human health. For NO2, ‘moderate’ air pollution (which requires action by citizens who are vulnerable to the health impacts of air pollution) is triggered when the latest hourly mean concentration is greater than 200 µg/m3. The coloured categories relate to the categories of the Daily Air Quality Index (see Table 20 in the statistical tables that accompany this release).





 ## Conversion

 https://www.kane.co.uk/knowledge-centre/what-does-nox-mean



## PIAQ

  float adc = analogRead(VRED_PIN);
  float Rs = 22000.0/(1023.0/adc);
  float R0 = 350.0;
  float ratio = Rs/R0;
  float concentration = (124.28*log(ratio))-270.37;
  Serial.print("NO2 concentration (ppb): ");
  //Serial.println(adc);
  //Serial.println(Rs);
  //Serial.println(R0);
  //Serial.println(ratio);
  Serial.println(concentration);
  return concentration;


```
Vnox: 354
NO2 concentration (ppb): 112.72

Vnox: 354
NO2 concentration (ppb): 112.37

Vnox: 353
NO2 concentration (ppb): 112.37

Vnox: 352
NO2 concentration (ppb): 112.02
``


  comparison

  http://sloughair.aeat.com/weekly.php?site=SLH10
http://sloughair.aeat.com/weekly.php?site=SLH7


  http://sloughair.aeat.com/weekly.php?site=SLH11 - pm10


  https://uk-air.defra.gov.uk/assets/documents/reports/cat06/0502160851_Conversion_Factors_Between_ppb_and.pdf

  On the UK Air Quality Archive, and for reporting data to the European Commission, the
appropriate conversion factors at 20oC and 1013 mb are used.

Nitrogen dioxide 1 ppb = 1.9125 µg m-3 1 ppb = 1.88 µg m-3

NO2 concentration (ppb): 116.85

https://uk-air.defra.gov.uk/assets/documents/reports/empire/lsoman/Appendix_I.pdf

1 ppb = 1.91 µg/m3
 1 µg/m3
 = 0.523 ppb 


so, 116.85 ppb = 223 µg m-3

bit high



  //https://github.com/Edvardoh/ArduinoAirQuality/blob/master/Arduino_NO2_CO_test/Arduino_NO2_CO_test.ino
  Vout = vnox/409.2; // take reading and convert ADC value to voltage
  Rs = 22000/((voltage/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 22kOhm load resistor
  ppbNO2 = (.000008*Rs - .0194)*1000;    //convert Rs to ppb concentration NO2 (equation derived from data found on http://airpi.es/sensors.php
  
  Serial.print("PPB NO2= ");
  Serial.println(ppbNO2);

vnox= 305
PPB NO2= 11.43 = 22 µg m-3

22


µg/m3 microgrammes per cubic metre



## AirPi
http://airpi.es/

uses MiCS-2710 - similar

uses resistor - might need to step down output voltage in ADC


ADC conversion
https://learn.sparkfun.com/tutorials/analog-to-digital-conversion/relating-adc-value-to-voltage

Gas conversion formulas for MICS (German)
https://myscope.net/auswertung-der-airpi-gas-sensoren/

var R0 = 1022000; // R0 bei Frischluft (geschätzt)
var ppm = Math.pow(10, 0.9682 * (Math.log(Rs/R0) / Math.LN10) - 0.8108);