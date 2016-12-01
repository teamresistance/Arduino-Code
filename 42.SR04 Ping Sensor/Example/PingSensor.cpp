#include "Arduino.h"
#include "PingSensor.h"

PingSensor::PingSensor(int triggerPin, int echoPin)
{	
  // The Trigger pin will tell the sensor to range find
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Save for later
  _trigPin = triggerPin;
  _echoPin = echoPin;
}

long PingSensor::GetDuration()
{
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  return pulseIn(_echoPin, HIGH, _duraTimeout);
}

float PingSensor::GetDistance(float kFactor)
{
  if (kFactor > 0.0) {
    return GetDuration() / kFactor;  // uSec / (uSec/Unit) = unit
  }else{
    return -1.0;
  }
}

void PingSensor::Set_DuraTimeout(long t)
{
  _duraTimeout = t;  // default = 23600 uSec
}

