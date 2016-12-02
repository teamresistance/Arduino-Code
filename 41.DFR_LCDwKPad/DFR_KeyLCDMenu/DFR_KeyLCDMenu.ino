/* Use DFRobot LCD Shield for Arduino
    Key Codes (in left-to-right order): or
None - 0(1023)
                Up-3(100)
Next-1(641) Sel-2(410) Esc-5(0)
                Dn-4(256)
*/

#include <LiquidCrystal.h>
#include "DFR_Key.h"
#include "Menu.h"
#include "DisplayLCD.h"

//Pin  for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

//Handles button presses
DFR_Key keypad;
int localKey = 0;
String keyString = "";
String keyArray[6] = {"0 - NO KEY", "1 - Next", "2 - Select", "3 - Up", "4 - Dn", "5 - Esc"};
unsigned long calKeyTimer = 0;

//Create Menus
Menu MainMenu(3);
String MainAr[3] = {"Defence", "Gate", "Direction"};
Menu DefMenu(5);
String DefAr[5] = {"Rough Terrain", "Water Xing", "Sally Port", "Tetter Totter", "Low Boy"};
Menu GateMenu(5);
String GateAr[5] = {"1(LB)", "2", "3", "4", "5"};
Menu DirMenu(2);
String DirAr[2] = {"Forward", "Backward"};
                
void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu System v0.1");
  delay(2500);

  Serial.begin(9600);   //Console troubleshooting
  
//Assign Analog Resistance Values
//  array          {None, UP, DWN, LEFT, RIGHT, SEL}
  int keyLimits[6] = {0, 100, 255, 410, 641, 1023}; // DFR ver 1.1
  keypad.set_KeyARV(keyLimits);
}

void loop() 
{ 
  localKey = keypad.getKey();
  
  if (localKey != SAMPLE_WAIT)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current Key:");
    lcd.setCursor(0, 1);

    lcd.print(analogRead(0));
    lcd.print(" : ");
    lcd.print(keyArray[localKey]);
  }

//------------- Calibration mode for buttons -------------------
  if (analogRead(0) == 0) {
    if (calKeyTimer == 0) calKeyTimer = millis() + 5000;
  }else{
    calKeyTimer = 0;
  }

  if (millis() > calKeyTimer && calKeyTimer != 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrate ARVs:");
    lcd.setCursor(0, 1);
    lcd.print("Press All Keys Once");

    for (int i = 0; i < 6; i++) {         // AVRs before calibration
      Serial.print(keypad.getKeyARV(i));
      Serial.print("\t");
    }
    Serial.println();

    keypad.calibrKeyARVs();

    for (int i = 0; i < 6; i++) {       // ARVs after calibration
      Serial.print(keypad.getKeyARV(i));
      Serial.print("\t");
    }
    Serial.println();
  }
}

