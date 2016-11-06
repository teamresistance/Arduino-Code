#include "Arduino.h"
#include "DFR_Key.h"

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

static int DFROBOT = 0;
static int CYTRON = 1;

static int UPKEY_ARV = 131;
static int DOWNKEY_ARV = 307;
static int LEFTKEY_ARV = 480;
static int RIGHTKEY_ARV = 0;
static int SELKEY_ARV = 723;
static int NOKEY_ARV = 1023;

DFR_Key::DFR_Key(int dev)
{	
  _refreshRate = 10;
  _keyPin = DEFAULT_KEY_PIN;
  _threshold = DEFAULT_THRESHOLD;
  _keyIn = NO_KEY;
  _curInput = NO_KEY;
  _curKey = NO_KEY;
  _prevInput = NO_KEY;
  _prevKey = NO_KEY;
  _oldTime = 0;
  
  if (def == DFRKEY)
  {
	  UPKEY_ARV = 144;
	  DOWNKEY_ARV = 329;
	  LEFTKEY_ARV = 505;
	  RIGHTKEY_ARV = 0;
	  SELKEY_ARV = 742;
	  NOKEY_ARV = 1023;
  }
  else if (def == CYTRON)
  {
	  UPKEY_ARV = 131;
	  DOWNKEY_ARV = 307;
	  LEFTKEY_ARV = 480;
	  RIGHTKEY_ARV = 0;
	  SELKEY_ARV = 723;
	  NOKEY_ARV = 1023;
  }
}

int DFR_Key::getKey()
{
 if (millis() > _oldTime + _refreshRate)
 {
    _prevInput = _curInput;
    _curInput = analogRead(_keyPin);
  
    if (_curInput == _prevInput)
    {
      _change = false;
      _curKey = _prevKey;
    }
    else
    {
      _change = true;
      _prevKey = _curKey;
  
      if (_curInput > UPKEY_ARV - _threshold && _curInput < UPKEY_ARV + _threshold ) _curKey = UP_KEY;
      else if (_curInput > DOWNKEY_ARV - _threshold && _curInput < DOWNKEY_ARV + _threshold ) _curKey = DOWN_KEY;
      else if (_curInput > RIGHTKEY_ARV - _threshold && _curInput < RIGHTKEY_ARV + _threshold ) _curKey = RIGHT_KEY;
      else if (_curInput > LEFTKEY_ARV - _threshold && _curInput < LEFTKEY_ARV + _threshold ) _curKey = LEFT_KEY;
      else if (_curInput > SELKEY_ARV - _threshold && _curInput < SELKEY_ARV + _threshold ) _curKey = SELECT_KEY;
      else _curKey = NO_KEY;
   }
   
   if (_change) return _curKey; else return SAMPLE_WAIT;
   _oldTime = millis();
 }
}

void DFR_Key::setRate(int rate)
{
  _refreshRate = rate;
}

void DFR_Key::setARV(int key, int dev)
{
	

}