#ifndef PingSensor_h
#define PingSensor_h

#include "Arduino.h"

#define KF_IN 148.0    // (2 * 74 uSec/in)
#define KF_CM 58.0     // (2 * 29 uSec/cm)

class PingSensor
{
  public:	
    PingSensor(int triggerPin, int echoPin);
	  float GetDistance(float kFactor);
	  void Set_unitKf(float uSecPerUnit);
	  void Set_DuraTimeout(long t);
  private:
	  long GetDuration();
    int _trigPin;
    int _echoPin;
    float _uSecPer_Unit = 148.0;  // default = (2 * 74 uSec/in)
//                               // 58.0 = (2 * 29 uSec/cm)
    long _duraTimeout = 23600;    // >400 cm
};

#endif
