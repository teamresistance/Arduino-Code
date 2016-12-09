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
const int PMAXITEMS = 3;
const String PrimeNames[PMAXITEMS] = {"Gate", "Def", "Dir"};
Menu PrimeMenu;

const int GATEITEMS = 5;
const String GateNames[GATEITEMS] = {"1(LB)", "2", "3", "4", "5"};
const int DEFITEMS = 5;
const String DefNames[DEFITEMS] = {"Rough Terrain", "Water Xing", "Sally Port", "Tetter Totter", "Low Boy"};
const int DIRITEMS = 4;
const String DirNames[DIRITEMS] = {"Forward", "Backward", "Left", "Right"};
Menu MyMenu[PMAXITEMS];

const int NOKEYPRSPRIMTM = 5;     //Seconds to display next Primary menu when in auto mode
const int NOKEYPRSSUBTM = 180;    //Seconds if no key pressed in sub menu to return to auto mode
unsigned long noKeyPrsTmr = 0;    //Timeout timer for No Key Pressed
bool noKeyPrs = false;            //No key pressed for some time
int pMenuPtr = 0;                 //Pointer to Primary menu
int prvPsubPtr = 0;               //Previous Prime sub Menu pointer
int pMenuAct;                     //Prime Menu Active pointer
int pSubMenu;                     //Prime Sub Menu pointer

String str1;                      //String for line 1 of LCD
String str2;                      //String for line 2 of LCD

void setup() 
{ 
  Serial.begin(9600);   //Console troubleshooting
  
//Assign Analog Resistance Values
//  array          {None, UP, DWN, LEFT, RIGHT, SEL}
const  int keyLimits[6] = {0, 100, 255, 410, 641, 1023}; // DFR ver 1.1
  keypad.set_KeyARV(keyLimits);

  PrimeMenu.SetMaxItems(PMAXITEMS);
  PrimeMenu.SetNames(PrimeNames);
  PrimeMenu.UpdateMenu(SEL_KEY);

  MyMenu[0].SetMaxItems(GATEITEMS);
  MyMenu[0].SetNames(GateNames);
  MyMenu[1].SetMaxItems(DEFITEMS);
  MyMenu[1].SetNames(DefNames);
  MyMenu[2].SetMaxItems(DIRITEMS);
  MyMenu[2].SetNames(DirNames);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu System v0.1");
  delay(1500);
}

void loop() 
{ 
/*  for (int i; i < 5; i++) {
    Serial.print(MyMenu[1].GetSubPtr());
    Serial.print("\t" + MyMenu[1].GetSubName());
    Serial.print("\t" + MyMenu[1].GetActName());
    Serial.println("\t" + DefNames[MyMenu[1].GetSubPtr()]);
  } */
  //delay(500);
  
  localKey = keypad.getKey();
  
  if (localKey > NO_KEY) {      //If key pressed, set timeout to 180 Sec.
    noKeyPrsTmr = millis() + (NOKEYPRSSUBTM * 1000L); //Wait 180 sec if no key prs
  }

  noKeyPrs = (millis() > noKeyPrsTmr);   //Check no key timeout
  if (noKeyPrs){            //If no key has been pressed for timeout
    localKey = NEXT_KEY;      //cancel actMenu or move to next primary display
    noKeyPrsTmr = millis() + (NOKEYPRSPRIMTM * 1000L); //Do again in 5 sec.
  }
  
  if (localKey > NO_KEY && localKey <= ESC_KEY) {            //Key pressed
    for (pMenuAct = 0; pMenuAct < PMAXITEMS; pMenuAct++) {
      if (MyMenu[pMenuAct].GetMenuMode() != MAINMENU) break;
    }
    if (pMenuAct < PMAXITEMS){       //A menu is active, pass key to Menu handler
      MyMenu[pMenuAct].UpdateMenu(localKey);
    }else {                   //A menu is not active, pass key to Primary Menu handler
      if (localKey == NEXT_KEY){        //--Next
        localKey = DN_KEY;
      }else if (localKey == SEL_KEY){   //--Select (Left)
        pMenuAct = PrimeMenu.GetSubPtr();
        MyMenu[pMenuAct].UpdateMenu(localKey);
      }else if (localKey == ESC_KEY) {
        noKeyPrsTmr = millis() + (NOKEYPRSPRIMTM * 1000L);
      }
      if (localKey == UP_KEY || localKey == DN_KEY) PrimeMenu.UpdateMenu(localKey);
    }
  }

//---------  Do Display  -----------------
  pSubMenu = PrimeMenu.GetSubPtr();         //Get updated values
  for (pMenuAct = 0; pMenuAct < PMAXITEMS; pMenuAct++) {
    if (MyMenu[pMenuAct].GetMenuMode() != MAINMENU) break;
  }
  if (prvKey != localKey || prvPsubPtr != pSubMenu){
    if (pMenuAct >= PMAXITEMS){             //Display as Prime Menu Active
      str1 = PrimeNames[pSubMenu] + "- " + MyMenu[pSubMenu].GetActName();
      pSubMenu++;
      if (pSubMenu >= PMAXITEMS) pSubMenu = 0; 
      str2 = PrimeNames[pSubMenu] + "- " + MyMenu[pSubMenu].GetActName();
    }else{                                  //Display as Sub Menu Active
      if (MyMenu[pMenuAct].GetMenuMode() == SUBMENU){
        str2 = "Sel?- ";
      }else{
        str2 = "Conf?- ";
      }
      str1 = "Act " + PrimeMenu.GetSubName() + "- " + MyMenu[pMenuAct].GetActName();
      str2 = str2 + MyMenu[pMenuAct].GetSubName();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(str1);
    lcd.setCursor(0, 1);
    lcd.print(str2);
  }

  prvKey = localKey;
  prvPsubPtr = PrimeMenu.GetSubPtr();

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

