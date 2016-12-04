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
// #include "DisplayLCD.h"

//Pin  for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

//Handles button presses
DFR_Key keypad;
int localKey = 0;
int prvKey = 0;
String keyString = "";
String keyArray[6] = {"0 - NO KEY", "1 - Next", "2 - Select", "3 - Up", "4 - Dn", "5 - Esc"};
unsigned long calKeyTimer = 0;

//Create Menus
const int GATEMENU = 0;
const int DEFMENU = 1;
const int DIRMENU = 2;
const int PMAXITEM = 3;

const String GateNames[5] = {"1(LB)", "2", "3", "4", "5"};
Menu GateMenu(5, GateNames);
const String DefNames[5] = {"Rough Terrain", "Water Xing", "Sally Port", "Tetter Totter", "Low Boy"};
Menu DefMenu(5, DefNames);
String DirNames[2] = {"Forward", "Backward"};
Menu DirMenu(2, DirNames);

bool aMenuAct = false;
int primMenuPtr = GATEMENU;
unsigned long noKeyPressTO = 0;
bool noKeyPress = false;

String str1;
String str2;

                
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

  if (localKey > NO_KEY) {      //If key pressed, set timeout to 180 Sec.
    noKeyPressTO = millis() + (180 * 1000);    
  }else { 
    if (noKeyPress){            //If no key has been pressed for timeout
      localKey = NEXT_KEY;      //cancel actMenu or move to next primary display
      noKeyPressTO = millis() + 5 * 1000; //Do again in 5 sec.
    }
  }
  noKeyPress = (millis() > noKeyPressTO);

  if (localKey > NO_KEY) {    //Key pressed
    aMenuAct = true;
    if (GateMenu.GetMenuMode() != MAINMENU) {
      GateMenu.UpdateMenu(localKey);
    }else if (DefMenu.GetMenuMode() != MAINMENU) {
      DefMenu.UpdateMenu(localKey);
    }else if (DirMenu.GetMenuMode() != MAINMENU) {
      DirMenu.UpdateMenu(localKey);
    } else {
      aMenuAct = false;
    }
  
    if (!aMenuAct) {
      switch (localKey) {
        case NEXT_KEY:              //--Next
        primMenuPtr++;              //Increment to Next Primary Menu
        if (primMenuPtr > PMAXITEM) primMenuPtr = 0;
        break;
        case SEL_KEY:               //--Select (Left)
        switch (primMenuPtr) {
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
          Serial.print("/nERR-11, primMenuPtr > 2, ");
          Serial.println(primMenuPtr);
          break;
        }
        break;
        case UP_KEY:                //--Up
        primMenuPtr--;              //Decrement to Prv Primary Menu
        if (primMenuPtr < 0) primMenuPtr = PMAXITEM;  //Wrap around
        break;
        case DN_KEY:                //--Dn
        primMenuPtr++;              //Increment to Next Primary Menu
        if (primMenuPtr > PMAXITEM) primMenuPtr = 0;   //Wrap araound
        break;
        case ESC_KEY:               //--Esc (Right)
        noKeyPressTO = millis();
        break;
        default:
        Serial.print("/nERR-1, localKey > 5, ");
        Serial.println(localKey);
        break;
      }
    }
  }

  
/*
  if (prvKey != localKey){
    lcd.clear();
    lcd.setCursor(0, 0);
    if (aMenuAct){
        switch (primMenuPtr) {
          case GATEMENU:            //Display Gate Menu
          str1 = "Act Gate - " & gateAr(GateMenu.GetActPtr())
          str2 = "Select? - " & gateAr(GateMenu.GetSubPtr())
          break;
          case DEFMENU:             //Display Defence Menu
          str1 = "Act Def - " & defAr(DefMenu.GetActPtr())
          str2 = "Select? - " & gateAr(DefMenu.GetSubPtr())
          break;
          case DIRMENU:             //Display Direction Menu
          str1 = "Act Dir - " & dirAr(DirMenu.GetActPtr())
          str2 = "Select? - " & gateAr(DirMenu.GetSubPtr())
          break;
          default:
          Serial.print("/nERR-11, primMenuPtr > 2, ");
          Serial.println(primMenuPtr);
          break;
        }
    }else {
        switch (primMenuPtr) {
          case GATEMENU:            //Display Gate Menu
          str1 = "Gate - " & gateAr(GateMenu.GetActPtr())
          str2 = "Def - " & defAr(DefMenu.GetActPtr())
          break;
          case DEFMENU:             //Display Defence Menu
          str1 = "Def - " & defAr(DefMenu.GetActPtr())
          str2 = "Dir - " & dirAr(DirMenu.GetActPtr())
          break;
          case DIRMENU:             //Display Direction Menu
          str1 = "Dir - " & dirAr(DirMenu.GetActPtr())
          str2 = "Gate - " & gateAr(GateMenu.GetActPtr())
          break;
          default:
          Serial.print("/nERR-11, primMenuPtr > 2, ");
          Serial.println(primMenuPtr);
          break;
        }
    }
    line1 = DisplayLCD(1);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    line2 = DisplayLCD(1);
    lcd.print(line2);
  }
  */
  prvKey = localKey;

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

