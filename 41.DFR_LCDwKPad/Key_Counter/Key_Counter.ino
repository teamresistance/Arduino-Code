/*
DFRobot LCD Shield for Arduino
Key Grab v0.4
Written by Glendon Klassen
gjklassen@gmail.com
http://www.sourceforge.net/users/ecefixer
http://ecefixer.tumblr.com

Special Thanks to keithspg@users.sf.net for Cytron device support ARVs.

Displays the currently pressed key on the LCD screen.

Key Codes (in left-to-right order):

None   - 0
Select - 1
Left   - 2
Up     - 3
Down   - 4
Right  - 5

*/

#include <LiquidCrystal.h>
#include <DFR_Key.h>

//Pin assignments for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

DFR_Key keypad;

int localKey = 0;
int prevNonWaitKey = 0;
int count = 0;
String keyString = "";String keyArray[6] = {"0 - NO KEY", "1 - SEL KEY", "2 - LEFT KEY", "3 - UP KEY", "4 - DN KEY", "5 - RIGHT KEY"};
                 
void setup() 
{ 
  Serial.begin(9600);      // open the serial port at 9600 bps:    

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Key Counter v0.5");
  delay(1000);
  
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
  keypad.getKey(); Grabs the current key.
  Returns -1 (sample wait) when no key toggled.
  Returns No key pressed - 0, Select - 1, Left - 2,
  Up - 3,Down - 4, Right - 5
  */
  localKey = keypad.getKey();

  if (localKey != SAMPLE_WAIT)
    {
    for (int i = 0; i< 6; i++) {
      Serial.print(keypad.getKeyAVR(i));
      Serial.print("\t");
    }
    Serial.println("");
      
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current Key:");
    lcd.setCursor(0, 1);

    lcd.print(analogRead(0));
    lcd.print(" : ");
    lcd.print(localKey);
  }
}
