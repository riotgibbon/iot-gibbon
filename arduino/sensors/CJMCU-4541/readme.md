# CJMCU-4541 / MICS-4514


https://www.banggood.com/CJMCU-4541-MICS-4514-Carbon-Monoxide-Nitrogen-Oxygen-Sensor-CONO2H2NH3CH4-p-1183263.html



https://forum.arduino.cc/index.php?topic=517952.0

https://github.com/ShawnHymel/MICS-4514_CO_and_NOx_Sensor_Breakout

#define PRE_PIN          8 # D1
#define VNOX_PIN         A0  # =VP / 36
#define VRED_PIN         A3  #  VN / 39



might be a https://wiki.seeedstudio.com/Grove-Multichannel_Gas_Sensor/

https://bochovj.wordpress.com/2019/08/15/airsense-3rd-data-collection-and-some-conclusions/
`The CJMCU 4541 NOx channel is equivalent to the Mics 2714.`

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

