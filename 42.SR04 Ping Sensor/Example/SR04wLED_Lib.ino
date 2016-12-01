/**
 * HC-SR04 Demo
 * Demonstration of the HC-SR04 Ultrasonic Sensor
 * Date: August 3, 2016
 * 
 * Description:
 *  Connect the ultrasonic sensor to the Arduino as per the
 *  hardware connections below. Run the sketch and open a serial
 *  monitor. The distance read from the sensor will be displayed
 *  in centimeters and inches.
 * 
 * Hardware Connections:
 *  Arduino | HC-SR04 
 *  -------------------
 *    5V    |   VCC     
 *    5     |   Trig     
 *    6     |   Echo     
 *    GND   |   GND
 *  
 * License:
 *  Public Domain
 */

#include "PingSensor.h"
#include "LED_Ctl.h"

// Ping Pins
const int TRIG_PIN = 5;
const int ECHO_PIN = 6;
PingSensor myPing(TRIG_PIN, ECHO_PIN);
  
float inches = 0.0;
float cm = 0.0;

// ---------  LED stuff  -----------
const int LED1_PIN = 3;
LED_Ctl myLED1(LED1_PIN, 0, 0); //For Ex. 1
const int LED2_PIN = 4;
LED_Ctl myLED2(LED2_PIN, 0, 0); //For Ex. 2

const int RED_PIN = 9;        //Only have 1 RGB LED
const int GRN_PIN = 10;       //Run Ex. 3 or 4 comment out the other
const int BLU_PIN = 11;
LED_Ctl myRGB34(RED_PIN, GRN_PIN, BLU_PIN); //For Ex. 3 & 4

int LEDval = 0;         //LED Cmd Value - Ex. 1, Fade On
float inches_LL = 6.0;  //Low Limit - Ex. 2, Off/On
float inches_DF = 2.0;  //Differential
bool LEDcmd;            //LED Command
int redVal = 0;         //RGB LED - Ex. 3, GRN to YEL to RED
int grnVal = 0;
int bluVal = 0;
unsigned char LEDcolor = 0; //Enum:0=Off,,WHT,GRN,LTB,BLU,VIO,YEL,7=RED - Ex. 4

void setup() {
// We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void loop() {

// ---------  Ping stuff  -----------
  inches = myPing.GetDistance(KF_IN);
  cm = myPing.GetDistance(KF_CM);

// ---------  LED stuff  -----------
// Example 1 - Single Integer - same as analogWrite
// analogWrite(LED1_PIN, constrain(x, 0, 255));  //Send LED cmd, 0 to 255
  LEDval = map(inches * 1000, 4000, 16000, 255, 0);  //Brighter as it moves Closer
  myLED1.Set_LEDCmd(LEDval);                   //Send LED cmd, 0 to 255

// Example 2 - Boolean - same as digitalWrite(LED_PIN, HIGH/LOW)
// digitalWrite(LED2_PIN, (inches < 6.0));  //LED on if less than 6"
  if (inches < inches_LL) LEDcmd = HIGH;
  if (inches > inches_LL + inches_DF) LEDcmd = LOW;
  myLED2.Set_LEDCmd(LEDcmd);               //LED on if less than 6", off at 8"

// Example 3 - Integer for 3 LEDs Red, Grn, Blue - 2 analogWrite's
  redVal = map(inches * 1000, 4000, 14000, 255, 0);
  grnVal = map(inches * 1000, 6000, 16000, 0, 255);
  bluVal = 0;
  myRGB34.Set_LEDCmd(redVal, grnVal, bluVal);  //Grn to Yel to Red

// Example 3 - Enum 7 colors for 3 LEDs. 0=OFF,WHT,GRN,LTB,BLU, VIO,YEL,7=RED
  LEDcolor = map(inches * 1000, 4000, 16000, 7, 0);  //OFF, WHT, ... , RED
//  myRGB34.Set_LEDCmd(LEDcolor);  //Chg color OFF,WHT,GRN,LTB,BLU, VIO,YEL,RED

  
  // Print out results
  if ( inches > 0.0 ) {
    Serial.print(inches);
    Serial.print(" in \t");
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(LEDcmd);
    Serial.print("\t");
    Serial.println(LEDval);
  } else {
    Serial.print("Out of range   ");
    Serial.println(inches);
  }
  
// Wait at least 100ms before next measurement
  delay(100);

}
