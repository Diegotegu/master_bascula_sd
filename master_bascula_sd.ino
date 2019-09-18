

//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library for two ore more HX711 modules
// Settling time (number of samples) and data filtering can be adjusted in the config.h file

#include <HX711_ADC.h>
#include <SD.h>
//HX711 constructor (dout pin(verde), sck pin(naranja))
HX711_ADC basc1(A1, A0); //HX711 1


boolean debugger = true;
const int chipSelect = 7;
float a;
long t = 0;
String datos = "";
File archivo;
String cabecera = "basc1";
String file_name="basc1_100gr.TXT";


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);// Configuring the onboard LED for status info (led on= sd card initialized // led flash= writing on SD
  digitalWrite(LED_BUILTIN, LOW);
  if (debugger) {
    Serial.begin(9600);
  }

  delay(10);

  if (debugger) {
    Serial.println();
    Serial.println("Starting...");
    Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
     }
    Serial.println("card initialized.");
  }
  if (SD.begin(chipSelect)) {
    digitalWrite(6, HIGH);
  }
  else while(1);

  archivo = SD.open(file_name, FILE_WRITE);
  archivo.println(cabecera);
  archivo.close();
  
  if (debugger) {
    Serial.println("archivo creado");
  }

  float calValue_1 = 1; // calibration value load cell 1



  basc1.begin();


  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time

  basc1.start(stabilisingtime);

 
  byte basc1_rdy = 0;

  
  if (debugger) {
    if (basc1.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.1 wiring and pin designations");
    }
  }
  
  basc1.setCalFactor(calValue_1); // user set calibration value (float)

  if (debugger) {
    Serial.println("Startup + tare is complete");
  }
}

void loop() {
  //update() should be called at least as often as HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS
  //longer delay in scetch will reduce effective sample rate (be carefull with use of delay() in the loop)


  //get smoothed value from data set + current calibration factor
for (int i=0;i<100;i++){
  basc1.update();
 
  delay(100);
    a += basc1.getData();
  
  }
  a/=100;
 
  datos = String(a);
  archivo = SD.open(file_name, FILE_WRITE);
  if (archivo) {
    archivo.println(datos);
    archivo.close();
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
    delay(250);                       // wait for a 1/4 second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on
    delay(250); 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
    delay(250);                       // wait for a 1/4 second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on
    delay(250); 
  }
  a=0;

  delay(10000);
}
