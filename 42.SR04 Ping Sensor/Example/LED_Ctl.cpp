#include "Arduino.h"
#include "LED_Ctl.h"

LED_Ctl::LED_Ctl(int RED_Pin = 0, int GRN_Pin = 0, int BLUE_Pin = 0)
{	
  pinMode(RED_Pin, OUTPUT);
  digitalWrite(RED_Pin, LOW);
  _REDPin = RED_Pin;
  _LEDPin = RED_Pin;
  
  pinMode(GRN_Pin, OUTPUT);
  digitalWrite(GRN_Pin, LOW);
  _GRNPin = GRN_Pin;

  pinMode(BLUE_Pin, OUTPUT);
  digitalWrite(BLUE_Pin, LOW);
  _BLUEPin = BLUE_Pin;
}

void LED_Ctl::Set_LEDCmd(bool LED_Cmd)
{
  digitalWrite(_LEDPin, LED_Cmd);
}

void LED_Ctl::Set_LEDCmd(int LED_Cmd)
{
  analogWrite(_LEDPin, constrain(LED_Cmd, 0, 255));
}

void LED_Ctl::Set_LEDCmd(int redCmd, int grnCmd, int blueCmd)
{
  analogWrite(_REDPin, constrain(redCmd, 0, 255));
  analogWrite(_GRNPin, constrain(grnCmd, 0, 255));
  analogWrite(_BLUEPin, constrain(blueCmd, 0, 255));
}

void LED_Ctl::Set_LEDCmd(unsigned char RBG_Cmd)
{
  switch (RBG_Cmd) {
    case OFF:
      digitalWrite(_REDPin, LOW);
      digitalWrite(_GRNPin, LOW);
      digitalWrite(_BLUEPin, LOW);
      break;
    case WHT:
      digitalWrite(_REDPin, HIGH);
      digitalWrite(_GRNPin, HIGH);
      digitalWrite(_BLUEPin, HIGH);
      break;
    case GRN:
      digitalWrite(_REDPin, LOW);
      digitalWrite(_GRNPin, HIGH);
      digitalWrite(_BLUEPin, LOW);
      break;
    case LTB:
      digitalWrite(_REDPin, LOW);
      digitalWrite(_GRNPin, HIGH);
      digitalWrite(_BLUEPin, HIGH);
      break;
    case BLU:
      digitalWrite(_REDPin, LOW);
      digitalWrite(_GRNPin, LOW);
      digitalWrite(_BLUEPin, HIGH);
      break;
    case VIO:
      digitalWrite(_REDPin, HIGH);
      digitalWrite(_GRNPin, LOW);
      digitalWrite(_BLUEPin, HIGH);
      break;
    case YEL:
      digitalWrite(_REDPin, HIGH);
      digitalWrite(_GRNPin, HIGH);
      digitalWrite(_BLUEPin, LOW);
      break;
    case RED:
      digitalWrite(_REDPin, HIGH);
      digitalWrite(_GRNPin, LOW);
      digitalWrite(_BLUEPin, LOW);
      break;
    default:
      digitalWrite(_REDPin, LOW);
      digitalWrite(_BLUEPin, LOW);
      digitalWrite(_GRNPin, LOW);
      break;
  }
}

