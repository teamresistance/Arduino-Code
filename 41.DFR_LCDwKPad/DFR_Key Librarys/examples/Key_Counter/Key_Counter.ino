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

const int DFRKEY = 0;
const int CYTRON = 1;

DFR_Key keypad(1);

int localKey = 0;
int prevNonWaitKey = 0;
int count = 0;
String keyString = "";
                 
void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Key Counter v0.4");
  delay(1000);
  
  /*
  OPTIONAL
  keypad.setRate(x);
  Sets the sample rate at once every x milliseconds.
  Default: 10ms
  */
  keypad.setRate(10);

}

void loop() 
{ 
  /*
  keypad.getKey();
  Grabs the current key.
  Returns a non-zero integer corresponding to the pressed key,
  OR
  Returns 0 for no keys pressed,
  OR
  Returns -1 (sample wait) when no key is available to be sampled.
  */
  localKey = keypad.getKey();
  
  if (localKey != SAMPLE_WAIT)
  {
    if (localKey != prevNonWaitKey)
    {
      if (localKey == UP_KEY)
      {
        count++;
      }
      else if (localKey == DOWN_KEY)
      {
        count--;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Current Count:");
      lcd.setCursor(0, 1);
      lcd.print(count);
      prevNonWaitKey = localKey;
    }
  }
}
