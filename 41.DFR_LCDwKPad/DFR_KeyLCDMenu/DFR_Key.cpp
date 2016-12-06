#include "Arduino.h"
#include "DFR_Key.h"

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

static int RIGHTKEY_ARV = 0;
static int UPKEY_ARV = 131;
static int DOWNKEY_ARV = 307;
static int LEFTKEY_ARV = 480;
static int SELKEY_ARV = 723;
static int NOKEY_ARV = 1023;

DFR_Key::DFR_Key()
{	
  _refreshRate = 500;
  _keyPin = DEFAULT_KEY_PIN;
  _threshold = DEFAULT_THRESHOLD;
  _keyIn = NO_KEY;
  _curInput = NO_KEY;
  _curKey = NO_KEY;
  _prvInput = NO_KEY;
  _prvKey = SAMPLE_WAIT;
  _nxTime = 0;
  _rptKeyTm = 500;
}

void DFR_Key::set_KeyARV(int keyARVs[])
{
  RIGHTKEY_ARV = keyARVs[0];
  UPKEY_ARV = keyARVs[1];
  DOWNKEY_ARV = keyARVs[2];
  LEFTKEY_ARV = keyARVs[3];
  SELKEY_ARV = keyARVs[4];
  NOKEY_ARV = keyARVs[5];
}

int DFR_Key::getKeyARV(int keyARVIndx)
{
  switch (keyARVIndx) {
    case 0:
	  return RIGHTKEY_ARV;  //keyARVs[0]
	  break;
    case 1:
	  return UPKEY_ARV;     //keyARVs[1]
	  break;
    case 2:
	  return DOWNKEY_ARV;	  //keyARVs[2]
	  break;
    case 3:
	  return LEFTKEY_ARV;	  //keyARVs[3]
	  break;
    case 4:
	  return SELKEY_ARV;	  //keyARVs[4]
	  break;
    case 5:
	  return NOKEY_ARV;	  //keyARVs[5]
	  break;
    default:
	  return -1;
  }
}

int DFR_Key::getKey()
{
  do {
    _curInput = analogRead(_keyPin);
    delayMicroseconds(10);
  } while (_curInput != analogRead(_keyPin));
  
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

  if (_curKey != _prvKey || _curKey == NO_KEY){
    _prvKey = _curKey;
    _rptKeyTmr = millis() + _rptKeyTm;
    return _curKey;
  }else{
    if (millis() > _rptKeyTmr){
      _rptKeyTmr = millis() + _rptKeyTm;
      return _curKey;
    }else{
      return SAMPLE_WAIT;
    }
  }
}

void DFR_Key::setRate(int rate)
{
  _refreshRate = rate;
}

void DFR_Key::calibrKeyARVs()
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
				break;
			}
		}
	} while (keyCalibr);

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
	
	set_KeyARV(keyVals);
}
