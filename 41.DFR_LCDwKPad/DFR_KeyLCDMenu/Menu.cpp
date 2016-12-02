#include "Arduino.h"
#include "Menu.h"

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

Menu::Menu()
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

void Menu::set_KeyARV(int keyARVs[])
{
  RIGHTKEY_ARV = keyARVs[0];
  UPKEY_ARV = keyARVs[1];
  DOWNKEY_ARV = keyARVs[2];
  LEFTKEY_ARV = keyARVs[3];
  SELKEY_ARV = keyARVs[4];
  NOKEY_ARV = keyARVs[5];
}

int Menu::getKeyARV(int keyARVIndx)
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

int Menu::getKey()
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

void Menu::setRate(int rate)
{
  _refreshRate = rate;
}

void Menu::calibrKeyARVs()
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
