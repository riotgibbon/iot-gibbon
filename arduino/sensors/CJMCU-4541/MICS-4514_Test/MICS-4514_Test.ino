#define PRE_PIN          8
#define VNOX_PIN         A0
#define VRED_PIN         A3

#define PRE_HEAT_SECONDS 10

int vnox_value = 0;
int vred_value = 0;
int NO2value = 0;
float NO2resistance;
float NO2seriesResistor = 22000;
float NO2measure = 0;

#define CALIB_R0_NO2      2200      // R0 calibration value for the NO2 sensor
#define CALIB_R0_CO       750000    // R0 calibration value for the CO sensor


void setup() {
  
  // Setup preheater pin
  pinMode(PRE_PIN, OUTPUT);
  
  // Initialize serial port
  Serial.begin(9600);
  Serial.println("MiCS-4514 Test Read");
  Serial.print("Preheating...");

  // Wait for preheating
  digitalWrite(PRE_PIN, 1);
  delay(PRE_HEAT_SECONDS * 1000);
  digitalWrite(PRE_PIN, 0);
  Serial.println("Done");
}

void loop() {
  
  // Read analog values, print them out, and wait
  vnox_value = analogRead(VNOX_PIN);
  vred_value = analogRead(VRED_PIN);
  Serial.print("Vnox: ");
  Serial.print(vnox_value, DEC);
  Serial.print(" Vred: ");
  Serial.println(vred_value, DEC);

  int NO2rawInput = vnox_value;
  NO2resistance = NO2seriesResistor * ((1023.0 / NO2rawInput) - 1.0);
  NO2measure = NO2resistance / 100;
  Serial.print(',');
  Serial.println(NO2measure);

      Serial.print(calculatePPM(vnox_value));
      Serial.println(" ppm");

         Serial.print(noxppm(vnox_value));
      Serial.println(" ppm");

    josephppm(vnox_value);
  delay(1000);
}


float calculatePPM(int analogVal) {
  float v = 3.3;
  float mvoltage = (analogVal * (v / 1023.0)) / 1000;
  float rawSensorVal = ((1.8 * 2) / mvoltage) - 2;
  float ppm = pow(10, (log10(rawSensorVal) - 2.176) / (-1.737));
  return (ppm);
}

float noxppm(int reading){
  // Convert reading to voltage (Volts)
  float fVolt, fRes, fConc;
   unsigned long calib;
  fVolt = 3.3;
  fVolt *= reading;
  fVolt /= 1024.0;

  // Get Rs/R0 value
  calib = CALIB_R0_NO2; 
  
  fRes = (5000.0/fVolt - 1000) / calib;

  // Convert to ppm
  if (fRes < 3.0)
    fRes = 3.0;
  if (fRes >= 3.0 && fRes < 8.0)
    fConc = (fRes - 0.5) / 0.25;
  else
    fConc = (fRes + 129.655) / 4.589;
  return fConc;
}

float josephppm(int adc0){
//  https://github.com/jo-seph/Mobisens/blob/master/mobisens7.6.ino
  float adc2=3.3;
    int Rload_no2     = 22000;
  int Rload_co      = 47000;
  float U_Rload_no2 = (adc0 * 0.0001875);
  float U_Board     = adc2 * 0.0001875; // Boardspannung
float I_no2       = (U_Rload_no2 / Rload_no2);
  float Rs_no2      = (U_Board - U_Rload_no2) / I_no2;
//  float Rs_co       = (U_Board - U_Rload_co ) / I_co ;

  int R0_no2 = 18000;
  
  
//  float U_Rload_no2 = (adc0 * 0.0001875);
//  float I_no2       = (U_Rload_no2 / Rload_no2);
  
  float RsR0_no2    = Rs_no2 / R0_no2;

// Serial.println(" ");
  Serial.print("Rs_no2 Ohm ");
  Serial.println(Rs_no2);

  // NoX in µg/m³   [ umr auf µg   *  ppm ]
  float noxppm = (pow(10, 0.993634072700759 * (log10(RsR0_no2) ) - 0.800900894085055));
  float nmue   = 1912.02966843778  * noxppm;

//    Serial.println(" ");
  Serial.print("noxppm ");
  Serial.println(noxppm);

}
