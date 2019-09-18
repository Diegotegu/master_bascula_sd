

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
HX711_ADC basc2(A3, A2); //HX711 2
HX711_ADC basc3(A5, A4); //HX711 3
HX711_ADC basc4(0, A6); //HX711 4
HX711_ADC basc5(2, 1); //HX711 5
HX711_ADC basc6(4, 3); //HX711 6

boolean debugger = false;
const int chipSelect = 7;
float a, b, c, d, e, f;
long t = 0;
String datos = "";
File archivo;
String cabecera = "basc1,basc2,basc3,basc4,basc5,basc6,sens1,sens2,sens3,sens4,sens5,sens6";
char resp;
String resp_1;
String resp_2;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);// Configuring the onboard LED for status info (led on= sd card initialized // led flash= writing on SD
  digitalWrite(LED_BUILTIN, LOW);
  if (debugger) {
    Serial.begin(9600);
  }
  Serial1.begin(9600);
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

  archivo = SD.open("datalog.txt", FILE_WRITE);
  archivo.println(cabecera);
  archivo.close();
  
  if (debugger) {
    Serial.println("archivo creado");
  }

  float calValue_1 = 1; // calibration value load cell 1
  float calValue_2 = 1; // calibration value load cell 2
  float calValue_3 = 1; // calibration value load cell 2
  float calValue_4 = 1; // calibration value load cell 2
  float calValue_5 = 1; // calibration value load cell 2
  float calValue_6 = 1; // calibration value load cell 2


  basc1.begin();
  basc2.begin();
  basc3.begin();
  basc4.begin();
  basc5.begin();
  basc6.begin();

  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time

  basc1.start(stabilisingtime);
  basc2.start(stabilisingtime);
  basc3.start(stabilisingtime);
  basc4.start(stabilisingtime);
  basc5.start(stabilisingtime);
  basc6.start(stabilisingtime);

  byte basc1_rdy = 0;
  byte basc2_rdy = 0;
  byte basc3_rdy = 0;
  byte basc4_rdy = 0;
  byte basc5_rdy = 0;
  byte basc6_rdy = 0;
  
  if (debugger) {
    if (basc1.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.1 wiring and pin designations");
    }
    if (basc2.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.2 wiring and pin designations");
    }
    if (basc3.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.3 wiring and pin designations");
    }
    if (basc4.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.4 wiring and pin designations");
    }
    if (basc5.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.5 wiring and pin designations");
    }
    if (basc6.getTareTimeoutFlag()) {
      Serial.println("Tare timeout, check MCU>HX711 no.6 wiring and pin designations");
    }
  }
  
  basc1.setCalFactor(calValue_1); // user set calibration value (float)
  basc2.setCalFactor(calValue_2); // user set calibration value (float)
  basc3.setCalFactor(calValue_3); // user set calibration value (float)
  basc4.setCalFactor(calValue_4); // user set calibration value (float)
  basc5.setCalFactor(calValue_5); // user set calibration value (float)
  basc6.setCalFactor(calValue_6); // user set calibration value (float)
  
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
  basc2.update();
  basc3.update();
  basc4.update();
  basc5.update();
  basc6.update();
  delay(100);
    a += basc1.getData();
    b += basc2.getData();
    c += basc3.getData();
    d += basc4.getData();
    e += basc5.getData();
    f += basc6.getData();
  }
  a/=100;
  b/=100;
  c/=100;
  d/=100;
  e/=100;
  f/=100;
  resp_1="";
  
  if (debugger) {
    Serial.println("enviando peticion");
  }
  
  Serial1.println("a");
  while(!Serial1.available());
  while (Serial1.available()) {
    resp = Serial1.read();
    resp_1 += resp;
    delay(100);
  }
  
  //    Serial.print("    Load_cell 1 output val: ");
  //    Serial.println(a);
  //    Serial.print("    Load_cell 2 output val: ");
  //    Serial.println(b);
  //    Serial.print("    Load_cell 3 output val: ");
  //    Serial.println(c);
  //    Serial.print("    Load_cell 4 output val: ");
  //    Serial.println(d);
  //    Serial.print("    Load_cell 5 output val: ");
  //    Serial.println(e);
  //    Serial.print("    Load_cell 6 output val: ");
  //    Serial.println(f);
  //    Serial.println();

  Serial1.println("b");
  while(!Serial1.available());
  while (Serial1.available()) {
    resp = Serial1.read();
    resp_2 += resp;
    delay(100);
  }

  if (debugger) {
    Serial.print(a);
    Serial.print(",");
    Serial.print(b);
    Serial.print(",");
    Serial.print(c);
    Serial.print(",");
    Serial.print(d);
    Serial.print(",");
    Serial.print(e);
    Serial.print(",");
    Serial.print(f);
    Serial.print(",");
    Serial.println(resp_1);
    Serial.println(resp_2);
  }

  datos = String(a) + "," + String(b) + "," + String(c) + "," + String(d) + "," + String(e) + "," + String(f) + "," + resp_1 + "," + resp_2;
  archivo = SD.open("datalog.txt", FILE_WRITE);
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
  b=0;
  c=0;
  d=0;
  e=0;
  f=0;

  delay(10000);
}
