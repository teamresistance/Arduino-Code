#ifndef DFR_Key_h
#define DFR_Key_h

#include "Arduino.h"

#define SAMPLE_WAIT -1
#define NO_KEY 0

class DFR_Key
{
  public:	
    DFR_Key();
    int getKey();             //Returns the key pressed
    void set_KeyARV(int[6]);  //None-0,Select-1,Left-2
       			                  //Up-3,Down-4,Right-5
    int getKeyARV(int);       //Gets the presently assigned AVR value
    void setRate(int);
    void calibrKeyARVs();     //Reads & assigns AVRs when button is pressed
  private:
    int _refreshRate;
    int _keyPin;
    int _threshold;
    int _keyIn;
    int _curInput;
    int _curKey;
    int _prevInput;
    int _prevKey;
    boolean _change;
    unsigned long _nxTime;
    int _tmpVal;
};

#endif
