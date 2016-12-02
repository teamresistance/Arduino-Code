/*
DFRobot LCD Shield for Arduino
Key Grab v0.2   Written by Glendon Klassen
gjklassen@gmail.com
http://www.sourceforge.net/users/ecefixer
http://ecefixer.tumblr.com

Key Codes (in left-to-right order): or

None - 0(1023)
                UP-3(100)
Sel-1(641) Left-2(410) Right-5(0)
                Dn-4(256)
*/

#include <LiquidCrystal.h>
#include "DFR_Key.h"

//Pin  for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

DFR_Key keypad;

int localKey = 0;
String keyString = "";
String keyArray[6] = {"0 - NO KEY", "1 - SEL KEY", "2 - LEFT KEY", "3 - UP KEY", "4 - DN KEY", "5 - RIGHT KEY"};
unsigned long calKeyTimer = 0;
                
void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Key Grab v0.2");
  delay(2500);

  Serial.begin(9600);
  
//  array          {None, UP, DWN, LEFT, RIGHT, SEL}
//  int keyLimits[6] = {0, 144, 329, 505, 742, 1023}; // DFR ver 1.0
  int keyLimits[6] = {0, 100, 255, 410, 641, 1023}; // DFR ver 1.1
//  int keyLimits[6] = {0, 131, 307, 408, 723, 1023}; // Cytron (default)
  keypad.set_KeyARV(keyLimits);
  
  /*
  OPTIONAL - keypad.setRate(x);
  Sets the sample rate at once every x milliseconds. Default: 10ms
  */
  keypad.setRate(10);

}

void loop() 
{ 
  /*
  keypad.getKey();  Grabs the current key.
  Returns a non-zero integer corresponding to the pressed key,
  OR Returns 0 for no keys pressed,
  OR Returns -1 (sample wait) when no key is available to be sampled.
  */
  localKey = keypad.getKey();
  
  if (localKey != SAMPLE_WAIT)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current Key:");
    lcd.setCursor(0, 1);

//    lcd.print(millis()/100);
    lcd.print(analogRead(0));
    lcd.print(" : ");
    lcd.print(keyArray[localKey]);
  }

  if (analogRead(0) == 0) {
    if (calKeyTimer == 0) calKeyTimer = millis() + 5000;
  }else{
    calKeyTimer = 0;
  }
    
  if (millis() > calKeyTimer && calKeyTimer != 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrate AVRs:");
    lcd.setCursor(0, 1);
    lcd.print("Press All Keys Once");

    for (int i = 0; i < 6; i++) {         // AVRs before calibration
      Serial.print(keypad.getKeyARV(i));
      Serial.print("\t");
    }
    Serial.println();

    keypad.calibrKeyARVs();

    for (int i = 0; i < 6; i++) {       // AVRs after calibration
      Serial.print(keypad.getKeyARV(i));
      Serial.print("\t");
    }
    Serial.println();

  }
}

