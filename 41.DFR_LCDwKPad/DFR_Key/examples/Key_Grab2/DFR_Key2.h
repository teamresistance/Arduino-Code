#ifndef DFR_Key_h
#define DFR_Key_h

#include "Arduino.h"

#define SAMPLE_WAIT -1
#define NO_KEY 0
#define UP_KEY 3
#define DOWN_KEY 4
#define LEFT_KEY 2
#define RIGHT_KEY 5
#define SELECT_KEY 1

class DFR_Key
{
  public:	
    DFR_Key();
    void set_KeyARV(int[6]); //None-0,Select-1,Left-2
							 //Up-3,Down-4,Right-5
    int getKeyAVR(int);
    int getKey();
    void setRate(int);
	void calibrKeyAVRs();
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