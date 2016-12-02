#include "Arduino.h"
#include "Menu.h"

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

Menu::Menu(int maxItems)  
  int _maxItems = maxItems    //Set the max # of items in the Menu
  int _menuMode = MAINMENU    //Used to determine how to handle Sel & Esc
  int _subMenuPtr = 0         //Points to subMenu choice
}

void UpdateMenu(int keyVal){   //Update submenu pointer or action
  if (keyVal < 0 && keyVal > _maxItems) {
    switch (keyVal) {
      case 1:               //--Next Prim
      _subMenuPtr = 0;      //Cancel Update
      _menuMode = 0;
      break;
      case 2:               //--Select (Left)
      switch (_menuMode) {
        case 0:             //-On Main
        _subMenuPtr = _actMenuPtr;  //Set to prv choice
        _menuMode = 1;      //and set mode to subMenu
        break;
        case 1:             //-On subMenu
          _menuMode = 3;    //Set to Confirm mode
        break;
        case 2:             //-Confirm mode
        _actMenuPtr = _subMenuPtr;  //Set actMenuPtr to subMenuPtr
        _menuMode = 0;      //and mode to Main
        break;
      }
      break;
      case 3:             //--Up
      _subMenuPtr-- ;     //Decrement subMenuPtr
      _menuMode = 1;
      break;
      case 4:             //--Dn
      _subMenuPtr++;      //Increment subMenuPtr
      _menuMode = 0;
      break;
      case 5:             //--Esc (Right)
      switch (_menuMode) {
        case 0:             //-On Main
        // do nothing.  Should be handled in main code
        break;
        case 1:             //-On subMenu
        _menuMode = 0;      //Set back to Main mode
        break;
        case 2:             //-Confirm mode
        _menuMode = 1;      //Set back to subMenu mode
        break;
      }
      break;
    }
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
