#include "Arduino.h"
#include "DFR_Key.h"

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

static int DFRKEY = 0;
static int CYTRON = 1;

static int RIGHTKEY_ARV = 0;
static int UPKEY_ARV = 131;
static int DOWNKEY_ARV = 307;
static int LEFTKEY_ARV = 480;
static int SELKEY_ARV = 723;
static int NOKEY_ARV = 1023;

DFR_Key::DFR_Key()
{	
  _refreshRate = 10;
  _keyPin = DEFAULT_KEY_PIN;
  _threshold = DEFAULT_THRESHOLD;
  _keyIn = NO_KEY;
  _curInput = NO_KEY;
  _curKey = NO_KEY;
  _prevInput = NO_KEY;
  _prevKey = NO_KEY;
  _nxTime = 0;
}

void DFR_Key::set_KeyARV(int keyAVRs[])
{
  RIGHTKEY_ARV = keyAVRs[0];
  UPKEY_ARV = keyAVRs[1];
  DOWNKEY_ARV = keyAVRs[2];
  LEFTKEY_ARV = keyAVRs[3];
  SELKEY_ARV = keyAVRs[4];
  NOKEY_ARV = keyAVRs[5];
}

int DFR_Key::getKeyAVR(int keyAVRIndx)
{
  switch (keyAVRIndx) {
    case 0:
	  return RIGHTKEY_ARV;  //keyAVRs[0]
	  break;
    case 1:
	  return UPKEY_ARV;     //keyAVRs[1]
	  break;
    case 2:
	  return DOWNKEY_ARV;	  //keyAVRs[2]
	  break;
    case 3:
	  return LEFTKEY_ARV;	  //keyAVRs[3]
	  break;
    case 4:
	  return SELKEY_ARV;	  //keyAVRs[4]
	  break;
    case 5:
	  return NOKEY_ARV;	  //keyAVRs[5]
	  break;
    default:
	  return -1;
  }
}

int DFR_Key::getKey()
{
 if (millis() > _nxTime)
 {
    _curInput = analogRead(_keyPin);
  
    if (_threshold > abs(_curInput - _prevInput))
    {
      _change = false;
      _curKey = _prevKey;
    }
    else
    {
      _change = true;
      _prevKey = _curKey;
	  _prevInput = _curInput;
/* ----------  Original version
      if (_curInput > UPKEY_ARV - _threshold && _curInput < UPKEY_ARV + _threshold ) _curKey = UP_KEY;
      else if (_curInput > DOWNKEY_ARV - _threshold && _curInput < DOWNKEY_ARV + _threshold ) _curKey = DOWN_KEY;
      else if (_curInput > RIGHTKEY_ARV - _threshold && _curInput < RIGHTKEY_ARV + _threshold ) _curKey = RIGHT_KEY;
      else if (_curInput > LEFTKEY_ARV - _threshold && _curInput < LEFTKEY_ARV + _threshold ) _curKey = LEFT_KEY;
      else if (_curInput > SELKEY_ARV - _threshold && _curInput < SELKEY_ARV + _threshold ) _curKey = SELECT_KEY;
      else _curKey = NO_KEY;
// ---------	  variation on original
	  _tmpVal = _curInput - _threshold;
      if (_tmpVal < RIGHTKEY_ARV) _curKey = RIGHT_KEY;
      else if (_tmpVal < UPKEY_ARV) _curKey = UP_KEY;
      else if (_tmpVal < DOWNKEY_ARV) _curKey = DOWN_KEY;
      else if (_tmpVal < LEFTKEY_ARV) _curKey = LEFT_KEY;
      else if (_tmpVal < SELKEY_ARV) _curKey = SELECT_KEY;
      else _curKey = NO_KEY;
// ----------	different choice  */	 
	  switch ((_curInput + 50) / 100) {
		  case 0:
			_curKey = RIGHT_KEY;
			break;
		  case 1:
			_curKey = UP_KEY;
			break;
		  case 2:
		  case 3:
			_curKey = DOWN_KEY;
			break;
		  case 4:
		  case 5:
			_curKey = LEFT_KEY;
			break;
		  case 6:
		  case 7:
			_curKey = SELECT_KEY;
			break;
		  default:
			_curKey = NO_KEY;
			break;
	  }
   }
   
   if (_change) return _curKey; else return SAMPLE_WAIT;
   _nxTime = millis() + _refreshRate;
 }
}

void DFR_Key::setRate(int rate)
{
  _refreshRate = rate;
}

void DFR_Key::calibrKeyAVRs()
{
	int localVal;
	int keyVals[6];
	bool keyCalibr = true;

	for (int i=0; i < 6; i++){
		keyVals[i] = -10;
	}

	do {
		do {
			localVal = analogRead(0);
			delay(10);		//debounce
		} while (abs(localVal - analogRead(0)) < _threshold);
		for (int i = 0; i < 6; i++) {
			if (abs(localVal - keyVals[i]) < _threshold) break;  // Already stored
			if (keyVals[i] == -10) {
				keyVals[i] = localVal;
				if (i == 5) keyCalibr = false;
				/*
				Serial.print("Stored-");
				Serial.print(localVal);
				Serial.print(" in ");
				Serial.println(i);
				*/
				break;
			}
		}
	} while (keyCalibr);
/* Print original order
	for (int i = 0; i < 6; i++) {
		Serial.print(keyVals[i]);
		Serial.print("\t");
	}
	Serial.println();
*/
	int tmp;
	bool sorted;
	do {
		sorted = true;
		for (int i = 0; i < 6; i++){
			if (keyVals[i] > keyVals[i+1]) {
				tmp = keyVals[i];
				keyVals[i] = keyVals[i+1];
				keyVals[i+1]=tmp;
				sorted = false;
			}
		}
	} while (!sorted);
	
/* Print sorted order
	for (int i = 0; i < 6; i++) {
		Serial.print(keyVals[i]);
		Serial.print("\t");
	}
	Serial.println();
*/	
	set_KeyARV(keyVals);
}