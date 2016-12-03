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
#define GATEMENU 0
#define DEFMENU 1
#define DIRMENU 2
Menu GateMenu(5);
String GateAr[5] = {"1(LB)", "2", "3", "4", "5"};
Menu DefMenu(5);
String DefAr[5] = {"Rough Terrain", "Water Xing", "Sally Port", "Tetter Totter", "Low Boy"};
Menu DirMenu(2);
String DirAr[2] = {"Forward", "Backward"};

bool aMenuAct = false;
int primMenuPtr = GATEMENU;
                
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

  if (localKey > NO_KEY) {
    keyNoPressTime = millis() + (180 * 1000);    
  }else { 
    if (keyNoPress){
      localKey = NEXT_KEY;
      keyNoPressTime = millis() + 5 * 1000;
    }
  }
  keyNoPress = (millis() > keyNoPressTime);

  if (localKey > NO_KEY) {    //Key pressed
    aMenuAct = true;
    if (GateMenu.GetMenuMode != MAINMENU) {
      GateMenu.UpdateMenu(localKey);
    }else if (DefMenu.GetMenuMode != MAINMENU) {
      DefMenu.UpdateMenu(localKey);
    }else if (DirMenu.GetMenuMode != MAINMENU) {
      DirMenu.UpdateMenu(localKey);
    } else {
      aMenuAct = false;
    }
  
    if (!aMenuAct) {
      switch (localKey) {
        case NEXT_KEY:              //--Next
        primMenuPtr++;              //Increment to Next Primary Menu
        if (primMenuPtr > DIRMENU) primMenu = GATEMENU;
        break;
        case SEL_KEY:               //--Select (Left)
        switch (primMenu) {
          case GATEMENU:            //Pass control to Gate Menu
          GateMenu.UpdateMenu(localKey);
          break;
          case DEFMENU:             //Pass control to Defence Menu
          DefMenu.UpdateMenu(localKey);
          break;
          case DIRMENU:             //Pass control to Direction Menu
          DirMenu.UpdateMenu(localKey);
          break;
          default:
          break;
        }
        break;
        case UP_KEY:                //--Up
        primMenuPtr--;              //Decrement to Prv Primary Menu
        if (primMenuPtr < GATEMENU) primMenuPtr = DIRMENU;  //Wrap around
        break;
        case DN_KEY:                //--Dn
        primMenuPtr++;              //Increment to Next Primary Menu
        if (primMenuPtr > DIRMENU) primMenu = GATEMENU;   //Wrap araound
        break;
        case ESC_KEY:               //--Esc (Right)
        autoMenuTime = millis();
        break;
        default:
        break;
      }
    }
  }
  
  
  if (localKey != SAMPLE_WAIT)
  {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    line1 = DisplayLCD(1);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    line2 = DisplayLCD(1);
    lcd.print(line2);
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

