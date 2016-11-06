#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


float AccelMinX = 0;
float AccelMaxX = 0;
float AccelMinY = 0;
float AccelMaxY = 0;
float AccelMinZ = 0;
float AccelMaxZ = 0;

char myChar;
int a;
  
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("ADXL345 Accelerometer Calibration"); 
  Serial.println("");

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
}

void loop(void)
{
    Serial.print("Type key when ready..."); 
    while (Serial.available() == 0){}   // wait for a character
    myChar = Serial.read();
    Serial.println(myChar);
    
    if (myChar == 'x' ) {
      Serial.print("x= "); Serial.println( GetOFS_X());
    }
    if (myChar == 'y' ) {
      Serial.print("y= "); Serial.println( GetOFS_Y());
    }
    if (myChar == 'z' ) {
      Serial.print("z= "); Serial.println( GetOFS_Z());
    }

    if (myChar == 'X' ) {
      SetOFS_X(0);
      delay(10);
      Serial.print("X= "); Serial.println( GetOFS_X());
    }
    if (myChar == 'Y' ) {
      SetOFS_Y(0);
      delay(10);
      Serial.print("Y= "); Serial.println( GetOFS_Y());
    }
    if (myChar == 'Z' ) {
      SetOFS_Z(0);
      delay(10);
      Serial.print("Z= "); Serial.println( GetOFS_Z());
    }

    delay(10);
    while (Serial.available())
    {
      Serial.read();  // clear the input buffer
    }
}

    uint8_t GetOFS_X()
    {
      return accel.readRegister(ADXL345_REG_OFSX);
    }

    uint8_t GetOFS_Y()
    {
      return accel.readRegister(ADXL345_REG_OFSY);
    }

    uint8_t GetOFS_Z()
    {
      return accel.readRegister(ADXL345_REG_OFSZ);
    }

    void SetOFS_X(uint8_t ofsVal)
    {
      return accel.writeRegister(ADXL345_REG_OFSX, ofsVal);
    }

    void SetOFS_Y(uint8_t ofsVal)
    {
      return accel.writeRegister(ADXL345_REG_OFSY, ofsVal);
    }

    void SetOFS_Z(uint8_t ofsVal)
    {
      return accel.writeRegister(ADXL345_REG_OFSZ, ofsVal);
    }

