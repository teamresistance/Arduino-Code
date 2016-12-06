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
const int PMAXITEM = 3;
const int GATEMENU = 0;
const int DEFMENU = 1;
const int DIRMENU = 2;

const String GateNames[5] = {"1(LB)", "2", "3", "4", "5"};
Menu GateMenu(5, GateNames);
const String DefNames[5] = {"Rough Terrain", "Water Xing", "Sally Port", "Tetter Totter", "Low Boy"};
Menu DefMenu(5, DefNames);
String DirNames[4] = {"Forward", "Backward", "Fwd", "Bkwd"};
Menu DirMenu(2, DirNames);

const int NOKEYPRSPRIMTM = 5;     //Seconds to display next Primary menu when in auto mode
const int NOKEYPRSSUBTM = 180;    //Seconds if no key pressed in sub menu to return to auto mode
unsigned long noKeyPrsTm = 0;   //Timeout timer for No Key Pressed
bool noKeyPress = false;          //No key pressed for some time
bool aMenuAct = false;            //A sub menu active
bool prvaMenuAct = false;
int primMenuPtr = GATEMENU;       //Pointer to Primary menu
unsigned long keyRptTm = 0;       //Time to repeat a key held down & debounce

String str1;
String str2;

                
void setup() 
{ 
  Serial.begin(9600);   //Console troubleshooting
  
//Assign Analog Resistance Values
//  array          {None, UP, DWN, LEFT, RIGHT, SEL}
const  int keyLimits[6] = {0, 100, 255, 410, 641, 1023}; // DFR ver 1.1
  keypad.set_KeyARV(keyLimits);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu System v0.1");
  delay(1500);

}

void loop() 
{ 
  localKey = keypad.getKey();
//  Serial.print("1- ");
//  Serial.print(localKey);
  
  if (localKey > NO_KEY) {      //If key pressed, set timeout to 180 Sec.
    noKeyPrsTm = millis() + (NOKEYPRSSUBTM * 1000L); //Wait 180 sec if no key prs
  }
//  Serial.print("    3-");
//  Serial.println((NOKEYPRSSUBTM * 1000L));
  delay(100);

  noKeyPress = (millis() > noKeyPrsTm);   //Check no key timeout
  if (noKeyPress){            //If no key has been pressed for timeout
    localKey = NEXT_KEY;      //cancel actMenu or move to next primary display
    noKeyPrsTm = millis() + NOKEYPRSPRIMTM * 1000; //Do again in 5 sec.
  }
  
  if (localKey > NO_KEY) {    //Key pressed
    aMenuAct = true;          //Initialize a Menu Active
    if (GateMenu.GetMenuMode() != MAINMENU) {       //Pass key to Gate if active
      GateMenu.UpdateMenu(localKey);
    }else if (DefMenu.GetMenuMode() != MAINMENU) {  //Pass key to Defence if active
      DefMenu.UpdateMenu(localKey);
    }else if (DirMenu.GetMenuMode() != MAINMENU) {  //Pass key to Direction if active
      DirMenu.UpdateMenu(localKey);
    } else {
      aMenuAct = false;       //No menu is active, pass key to Primary Menu handler
    }
  
    if (!aMenuAct) {                //Primary Menu handler
      switch (localKey) {
        case NEXT_KEY:              //--Next
          primMenuPtr++;              //Increment to Next Primary Menu
          if (primMenuPtr >= PMAXITEM) primMenuPtr = 0; //Wrap around
          break;
        case SEL_KEY:               //--Select (Left)
          switch (primMenuPtr) {
            case GATEMENU:            //Pass control to Gate Menu
              GateMenu.UpdateMenu(localKey);
              aMenuAct = true;
              break;
            case DEFMENU:             //Pass control to Defence Menu
              DefMenu.UpdateMenu(localKey);
              aMenuAct = true;
              break;
            case DIRMENU:             //Pass control to Direction Menu
              DirMenu.UpdateMenu(localKey);
              aMenuAct = true;
              break;
            default:
  //          Serial.print("\nERR-11, primMenuPtr > 2, ");
  //          Serial.println(primMenuPtr);
              break;
          }
          break;
        case UP_KEY:                //--Up
          primMenuPtr--;              //Decrement to Prv Primary Menu
          if (primMenuPtr < 0) primMenuPtr = (PMAXITEM - 1);  //Wrap around
          break;
        case DN_KEY:                //--Dn
          primMenuPtr++;              //Increment to Next Primary Menu
          if (primMenuPtr >= PMAXITEM) primMenuPtr = 0;   //Wrap araound
          break;
        case ESC_KEY:               //--Esc (Right)
          noKeyPrsTm = millis();
          break;
        default:
//        Serial.print("\nERR-1, localKey > 5, ");
//        Serial.println(localKey);
          break;
      }
    }
  }

//---------  Do Display  -----------------
  if (prvKey != localKey || prvaMenuAct == aMenuAct){
    if (aMenuAct){            //Display as Sub Menu
        switch (primMenuPtr) {
          case GATEMENU:            //Display Gate Menu
            if (GateMenu.GetMenuMode() == SUBMENU){
              str2 = "Sel? - ";
            }else{
              str2 = "Conf? - ";
            }
            str1 = "Act Gate - " + GateNames[GateMenu.GetActPtr()];
            str2 = str2 + GateNames[GateMenu.GetSubPtr()];
            break;
          case DEFMENU:             //Display Defence Menu
            if (DefMenu.GetMenuMode() == SUBMENU){
              str2 = "Sel? - ";
            }else{
              str2 = "Conf? - ";
            }
            str1 = "Act Def - " + DefNames[DefMenu.GetActPtr()];
            str2 = str2 + DefNames[DefMenu.GetSubPtr()];
            break;
          case DIRMENU:             //Display Direction Menu
            Serial.println("Dir Here");
            if (DirMenu.GetMenuMode() == SUBMENU){
              str2 = "Sel? - ";
            }else{
              str2 = "Conf? - ";
            }
            str1 = "Act Dir - " + DirNames[DirMenu.GetActPtr()];
            str2 = str2 + DirNames[DirMenu.GetSubPtr()];
            break;
          default:
//          Serial.print("/nERR-11, primMenuPtr > 2, ");
//          Serial.println(primMenuPtr);
            break;
        }
    }else {             //Display as Primary Menu
        switch (primMenuPtr) {
          case GATEMENU:            //Display Gate Menu
            str1 = "Gate - " + GateNames[GateMenu.GetActPtr()];
            str2 = "Def - " + DefNames[DefMenu.GetActPtr()];
            break;
          case DEFMENU:             //Display Defence Menu
            str1 = "Def - " + DefNames[DefMenu.GetActPtr()];
            str2 = "Dir - " + DirNames[DirMenu.GetActPtr()];
            break;
          case DIRMENU:             //Display Direction Menu
            str1 = "Dir - " + DirNames[DirMenu.GetActPtr()];
            str2 = "Gate - " + GateNames[GateMenu.GetActPtr()];
            break;
          default:
//          Serial.print("/nERR-11, primMenuPtr > 2, ");
//          Serial.println(primMenuPtr);
            break;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(str1);
    lcd.setCursor(0, 1);
    lcd.print(str2);
  }

  prvKey = localKey;
  prvaMenuAct = aMenuAct;

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

