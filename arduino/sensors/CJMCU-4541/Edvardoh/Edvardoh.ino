//https://github.com/Edvardoh/ArduinoAirQuality/blob/master/Arduino_NO2_CO_test/Arduino_NO2_CO_test.ino

const int NO2=A0; //NO2 sensor on analog 3
//const int CO=A3; //CO sensor on analog 2
float Vout = 0; //output voltage
float Rs = 0; //Sensor Resistance
float ppbNO2 = 0; //ppb NO2
float ppmCO = 0; //ppm CO
float voltage = 3.3;

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Starting debugging");
}

void loop()
{  
  // read NO2 sensor:
  int vnox = analogRead(NO2);
  Serial.print("vnox= ");
  Serial.println(vnox);
  
  Vout = vnox/409.2; // take reading and convert ADC value to voltage
  Rs = 22000/((voltage/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 22kOhm load resistor
  ppbNO2 = (.000008*Rs - .0194)*1000;    //convert Rs to ppb concentration NO2 (equation derived from data found on http://airpi.es/sensors.php
  
  Serial.print("PPB NO2= ");
  Serial.print(ppbNO2);
  Serial.print(", µg/m3 = ");
   Serial.println(ppbNO2 * 1.91);
//  // read CO sensor:
//  Vout = analogRead(CO)/409.2; // take reading and convert ADC value to voltage
//  Rs = 100000/((voltage/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 100kOhm load resistor
//  ppmCO = 911.19*pow(2.71828,(-8.577*Rs/100000));    //convert Rs to ppm concentration CO
//  
//  Serial.print("PPM CO= ");
//  Serial.println(ppmCO);

  delay(1000);
}
