#ifndef LED_Ctl_h
#define LED_Ctl_h

#include "Arduino.h"

#define KF_IN 148.0    // (2 * 74 uSec/in)
#define KF_CM 58.0     // (2 * 29 uSec/cm)
const unsigned char  OFF = 0;
const unsigned char  WHT = 1;
const unsigned char  GRN = 2;
const unsigned char  LTB = 3;
const unsigned char  BLU = 4;
const unsigned char  VIO = 5;
const unsigned char  YEL = 6;
const unsigned char  RED = 7;

class LED_Ctl
{
  public:	
    LED_Ctl(int RED_Pin, int GRN_Pin, int BLUE_Pin);
	  void Set_LEDCmd(int LED_Cmd);
    void Set_LEDCmd(int redVal, int grnVal, int blueVal);
    void Set_LEDCmd(bool LED_Cmd);
    void Set_LEDCmd(unsigned char RGB_Cmd);
  private:
    int _LEDPin;
    int _REDPin;
    int _GRNPin;
    int _BLUEPin;
};

#endif
