#include <Arduino.h>
#include <EEPROM.h>                 //SYSTEM PARAMETER  - EEPROM Library (By: Arduino)
#include <Wire.h>                   //SYSTEM PARAMETER  - WIRE Library (By: Arduino)
#include <SPI.h>                    //SYSTEM PARAMETER  - SPI Library (By: Arduino) 
#include <Adafruit_ADS1X15.h>
TaskHandle_t Core2;                 //SYSTEM PARAMETER  - Used for the ESP32 dual core operation
Adafruit_ADS1115 ads;   /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use this for the 12-bit version *

#define boost_IN         33          //SYSTEM PARAMETER - Buck MOSFET Driver PWM Pin
#define boost_EN         23          //SYSTEM PARAMETER - Buck MOSFET Driver Enable Pin

int PWM = 0, PPWM = 0, pwmMax = 0, pwmMaxLimited = 0, maxCurrent = 2, a = 0, PWM_MaxDC = 97 ;
float Sensitivity = .057;  
float offsetvoltage = 2.500;
int Sensorvalue = 0;

int pwmResolution           = 11;          //  USER PARAMETER - PWM Bit Resolution (can be set from 1 to 16)
int pwmFrequency            = 39000;       //  USER PARAMETER - PWM Switching Frequency - Hz (For Boost)
bool boostEnable = 0;;

float vo = 0.0, vi = 0.0,IC = 0.0, OC = 0.0, vomax = 80, cv = 0.0, Pin = 0.0, Pout = 0.0, ef = 0.0, eff = 0.0, efficiencyRate = 0.98, PPWM_margin = 95, PinPrev = 0, viPrev = 0;

int avgCountVS = 3, avgCountCS = 4;

double Voltage = 0;
double Current = 0;

bool z = 0;
bool pwmChannel            = 0;           // SYSTEM PARAMETER -(can be set from 0 to 15)

float VSI = 0.0000, VSO = 0.0000, CSI = 0.0000, inVoltageDivRatio = 59.92, outVoltageDivRatio = 59.95; 

unsigned long loopTimeStart = 0, loopTimeEnd = 0, loopTime = 0;

void readSensors(){
  
   /////////// VOLTAGE & CURRENT SENSORS /////////////
   VSI = 0.0000;      //Clear Previous Input Voltage 
   VSO = 0.0000;      //Clear Previous Output Voltage  
   CSI = 0.0000;      //Clear Previous Current  

  //VOLTAGE SENSOR - Instantenous Averaging   
  for(int i = 0; i<avgCountVS; i++){
    VSI = VSI + ads.computeVolts(ads.readADC_SingleEnded(0));
    VSO = VSO + ads.computeVolts(ads.readADC_SingleEnded(2));
  }
  vi  = (VSI/avgCountVS)*inVoltageDivRatio; 
  vo = (VSO/avgCountVS)*outVoltageDivRatio; 

  
  //CURRENT SENSOR - Instantenous Averaging   
  for(int i = 0; i<avgCountCS; i++){
    CSI = CSI + ads.computeVolts(ads.readADC_SingleEnded(1));
  }
  
   Voltage = CSI/avgCountCS;
  IC = (( offsetvoltage - Voltage  ) / Sensitivity);
  //delay(1);

  
 if(vo<=0.3) {vo=0;}
 if(vi<=0.3) {vi=0;}

 
 if(IC<=0.0) {IC=0;}
 
 OC = ((vi * IC)/vo) * efficiencyRate; //---->>>>Calculating the output current rather than measuring it<<<<----

 Pin = vi * IC;
 Pout = vo * OC;

if(Pin != 0 && Pout!=0){
 ef = (Pout/Pin) * 100;
  /* 
 a++;
 
 if(a == 10){

  eff = (eff*100)/10;
  if(eff>100){eff = 98;}
 // Serial.println(eff);
 
  cv = (cv+eff)/2;
 
  Serial.print("Efficiency: ");
  Serial.print(cv);
  Serial.println(" %");
  Serial.print("Vin: ");
  Serial.print(vi);
  Serial.println("V ");
  Serial.print("Iin: ");
  Serial.print(IC);
  Serial.println("A ");
  Serial.print("Vout: ");
  Serial.print(vo);
  Serial.println("V ");
  Serial.print("Iout: ");
  Serial.print(OC);
  Serial.println("A");
  Serial.print("Pin: ");
  Serial.print(Pin);
  Serial.println("W");
  Serial.print("Pout: ");
  Serial.print(Pout);
  Serial.println("W"); 

  cv = eff;
  eff = 0;
  ef = 0;
  a=0;

 // delay(10);
  
 }

 eff += ef;
*/
 }

}

void deviceProtection(){
    
    if(vi < 20 || vi > 180){ boost_Disable(); }
    //else if(OC > maxCurrent){ boost_Disable(); }
    else { boost_Enable(); }

}

void boost_Enable(){                                                                  //Enable MPPT Buck Converter
  boostEnable = 1;
  digitalWrite(boost_EN,HIGH);
  //digitalWrite(LED,HIGH);
}
void boost_Disable(){                                                                 //Disable MPPT Buck Converter
  boostEnable = 0; 
  digitalWrite(boost_EN,LOW);
 // digitalWrite(LED,LOW);
  PWM = 0;
}   
void predictivePWM(){                                                                //PREDICTIVE PWM ALGORITHM 
  if(vi<20){PPWM=0;}                                                       //Prevents Indefinite Answer when voltageInput is zero
  else{//PPWM =(PPWM_margin*pwmMax*vo)/(100.00*vi);
  //vi = 40;
  PPWM = ((1-(vi/vomax))*PPWM_margin*pwmMax)/100;}              //Compute for predictive PWM Floor and store in variable
  PPWM = constrain(PPWM,0,pwmMaxLimited);
}    

void PWM_Modulation(){
  if(boostEnable == 1){
    
    predictivePWM();                                                                   //Runs and computes for predictive pwm floor
     PWM = constrain(PWM,PPWM,pwmMaxLimited);                                         //CHARGER MODE PWM - limit floor to PPWM and ceiling to maximim allowable duty cycle)                                                                          
   // PWM = 0.46*pwmMax;
  ledcWrite(pwmChannel,PWM);                                                         //Set PWM duty cycle and write to GPIO when boost is enabled
  boost_Enable();  
  }
  else {
    PWM = 0;
    ledcWrite(pwmChannel,PWM);
  }                                                                   
}


/////////This code is for direct input and output. We want to increase pulse width same as generated at the gate of mosfet.//////////

void MPPTalgorithm(){

  if(OC > maxCurrent)                    {PWM--;}
  else if (vo > vomax)                   {PWM--;}
 
  else 
  {
    if (Pin > PinPrev && vi > viPrev)         {PWM--;}
    else if (Pin > PinPrev && vi < viPrev)    {PWM++;}
    else if (Pin < PinPrev && vi > viPrev)    {PWM++;}
    else if (Pin < PinPrev && vi < viPrev)    {PWM--;}
    else  if (vo < vomax)                     {PWM++;}
    PinPrev = Pin;
    viPrev = vi;

  }
 
 PWM_Modulation();
  

}



/////////This code is for opposite input and output. As pulse width generated increases, the pulse width decreases at gate of mosfet. 
/////////**** The pulse width generated and received is simplt the inverse of each other. ****
/////////It is because of the circuit of optocoupler.
/*
void MPPTalgorithm(){
  if(boostEnable == 1){
  if(OC > maxCurrent)                    {PWM++;}
  else if (vo > vomax)                   {PWM++;}
 
  else 
  {
    if (Pin > PinPrev && vi > viPrev)         {PWM++;}
    else if (Pin > PinPrev && vi < viPrev)    {PWM--;}
    else if (Pin < PinPrev && vi > viPrev)    {PWM--;}
    else if (Pin < PinPrev && vi < viPrev)    {PWM++;}
    else  if (vo < vomax)                     {PWM--;}
    PinPrev = Pin;
    viPrev = vi;

  }
 
 PWM_Modulation();
  }

  else {
    PWM = 0;
    ledcWrite(pwmChannel,PWM);
  } 

}
*/
//================== CORE1: SETUP (DUAL CORE MODE) ====================//

void setup() {

  Serial.begin(9600);

  pinMode(boost_EN,OUTPUT);

  //PWM INITIALIZATION
  ledcAttachPin(boost_IN, pwmChannel);                        //Set pin as PWM
  ledcSetup(pwmChannel,pwmFrequency,pwmResolution);          //Set PWM Parameters
  ledcWrite(pwmChannel,PWM);                                //Write PWM value at startup (duty = 0)

  pwmMax = pow(2,pwmResolution)-1;                           //Get PWM Max Bit Ceiling
  pwmMaxLimited = (PWM_MaxDC * pwmMax)/100.000;             //Get maximum PWM Duty Cycle (pwm limiting protection)
  
   // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  //ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  /*if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }*/
   
    //GPIO INITIALIZATION                          
  boost_Disable();

  //ENABLE DUAL CORE MULTITASKING
 // xTaskCreatePinnedToCore(coreTwo,"coreTwo",10000,NULL,0,&Core2,0);
 
  

}

void loop() {

  

  readSensors();
  deviceProtection();
  MPPTalgorithm();
    //////////// LOOP TIME STOPWATCH ////////////
  loopTimeStart = micros();                                            //Record Start Time
  loopTime = (loopTimeStart-loopTimeEnd)/1000.000;                     //Compute Loop Cycle Speed (mS)
  loopTimeEnd = micros();                                              //Record End Time

}