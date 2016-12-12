// Test the ADNS3080 Optical Flow Sensor
// based on: Example of AP_OpticalFlow library by Randy Mackay. DIYDrones.com


#include "SPI.h"
#include "ADNS3080.h"

//                                   MSTR    Chip
#define AP_SPI_DATAOUT         11  //MOSI    MISO
#define AP_SPI_DATAIN          12  //MISO    MOSI
#define AP_SPI_CLOCK           13  //SCK     SCLK
#define ADNS3080_RESET          9  //RESET   RESET
#define ADNS3080_CHIP_SELECT1   5  //SS      nSEL
#define ADNS3080_CHIP_SELECT2   6  //SS      nSEL

byte orig_spi_settings_spcr;
byte orig_spi_settings_spsr;
int _cs_pin=ADNS3080_CHIP_SELECT1;  //Will need to be set to desired chip select in code
int _reset_pin=1; // set to 1 if you have reset connected
unsigned int last_update;
boolean _overflow=false;
boolean _motion=false;
int raw_dx;
int raw_dy;
unsigned int surface_quality;

int i, j;

void setup() 
{
  Serial.begin(115200);
  Serial.println("www.bot-thoughts.com\nOptical Flow test program V1.0\n");
  
  delay(1000);
	
  // flowSensor 1 initialization
  _cs_pin = ADNS3080_CHIP_SELECT1;
  if( initOF() == false )
    Serial.println("Failed to initialise ADNS3080 One");
    Serial.println();
    delay(200);

  // flowSensor 2 initialization
  _cs_pin=ADNS3080_CHIP_SELECT2;
  if( initOF() == false )
    Serial.println("Failed to initialise ADNS3080 Two");
  delay(200);
}

void loop() 
{
  int value;
	
  display_menu();

  // wait for user to enter something
  while( !Serial.available() ) {
    delay(20);
  }

  // get character from user
  value = Serial.read();
	
  switch( value ) {
	
  case 'c' :
    _cs_pin = ADNS3080_CHIP_SELECT1;
    display_config();
    break;
			
  case 'v' :
    _cs_pin = ADNS3080_CHIP_SELECT2;
    display_config();
    break;
      
  case 'f' :
    //set_frame_rate();
    break;
			
  case 'u' :
    // display sensor 1 image
    _cs_pin = ADNS3080_CHIP_SELECT1;
    display_image();
    break;
			
  case 'i' :
    _cs_pin = ADNS3080_CHIP_SELECT1;
    display_image_continuously();
    break;
			
  case 'o' :
    _cs_pin = ADNS3080_CHIP_SELECT1;
    display_motion();
    break;
			
  case 'j' :
    // display sensor 2 image
    _cs_pin = ADNS3080_CHIP_SELECT2;
    display_image();
    break;
      
  case 'k' :
    _cs_pin = ADNS3080_CHIP_SELECT2;
    display_image_continuously();
    break;
      
  case 'l' :
    _cs_pin = ADNS3080_CHIP_SELECT2;
    display_motion();
    break;
      
//  case 'b' :
//    _cs_pin = ADNS3080_CHIP_SELECT2;
//    display_both_image_continuously();
//    break;
      
  case 'r' :
    // set resolution
    //set_resolution();
    break;
			
  case 's' :
    //set_shutter_speed();
    break;
			
  case 'z' :
    //flowSensor.clear_motion();
    break;		
    
  case '\r' : // ignore return type characters
  case '\n' :
    break;
			
  default:
    Serial.println("unrecognised command");
    Serial.println();
    break;	
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DISPLAY FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------------
// Prints out a list of functions.
//-------------------------------------------------------------------------------------------------------------------------
void display_menu()
{
    Serial.println();
    Serial.println("please choose from the following options:");
	Serial.println("     c - display sensor 1 all config");
  Serial.println("     v - display sensor 2 all config");
//	Serial.println("     f - set frame rate");
	Serial.println("     u - display sensor 1 image");
	Serial.println("     i - display sensor 1 image continuously");
	Serial.println("     o - display sensor 1 motion");	
  Serial.println("     j - display sensor 2 image");
  Serial.println("     k - display sensor 2 image continuously");
  Serial.println("     l - display sensor 2 motion"); 
//	Serial.println("     r - set resolution");
//	Serial.println("     s - set shutter speed");
//	Serial.println("     z - clear all motion");
//	Serial.println("     a - frame rate auto/manual");
	Serial.println();
}


//-------------------------------------------------------------------------------------------------------------------------
// Captures and displays image from flowSensor
//-------------------------------------------------------------------------------------------------------------------------
void display_image()

{	
  Serial.println("image data --------------");
  print_pixel_data(&Serial);
  Serial.println("-------------------------");
}

//-------------------------------------------------------------------------------------------------------------------------
// display_image continuously - captures and displays image from flowSensor
//-------------------------------------------------------------------------------------------------------------------------
void display_image_continuously()
{	
  int i;
  Serial.println("press any key to return to menu");

//  Serial.flush();  //flush def chged.  myFlush empties buffer.
  myFlush();
	
  while( !Serial.available() ) {
  display_image();
  i=0;
    while( i<20 && !Serial.available() ) {
      delay(100);  // give the viewer a bit of time to catchup
      i++;
    }
  }
    
//  Serial.flush();  //flush def chged.  myFlush empties buffer.
  myFlush();
}

//-------------------------------------------------------------------------------------------------------------------------
// display_both_image continuously - captures and displays image from flowSensor
//-------------------------------------------------------------------------------------------------------------------------
/* void display_both_image_continuously()
{  

  byte pixelVal2[30][30];

  int cntSpace=0;

  Serial.println("press any key to return to menu");

//  Serial.flush();  //flush def chged.  myFlush empties buffer.
  myFlush();

  _cs_pin = ADNS3080_CHIP_SELECT1;
  display_image();

  // copy chip1 pixel data
  for( i=0; i<ADNS3080_PIXELS_Y; i++ ) {
    for( j=0; j<ADNS3080_PIXELS_X; j++ ) {
      pixelVal2[i][j] = pixelVal1[i][j];
//      pixelVal2[][] = pixelVal1[][];
    }
  }
    

//  Serial.flush();  //flush def chged.  myFlush empties buffer.
  myFlush();

  _cs_pin = ADNS3080_CHIP_SELECT2;
  display_image();
    
  // display saved pixelVal2, Chip1 (Left) then pixelVal1, Chip2 (Right) data
  for( i=0; i<ADNS3080_PIXELS_Y; i++ ) {
    cntSpace=20;
    for( j=0; j<ADNS3080_PIXELS_X; j++ ) {
      Serial.print(pixelVal2[i][j]);
      if( j!= ADNS3080_PIXELS_X-1 )
        Serial.print(",");
      if ((pixelVal2[i][j]) < 100) cntSpace++;
      if ((pixelVal2[i][j]) < 10) cntSpace++;
    }
    for (; cntSpace>0; cntSpace-- ) Serial.print(" ");
    for( j=0; j<ADNS3080_PIXELS_X; j++ ) {
      Serial.print(pixelVal1[i][j]);
      if( j!= ADNS3080_PIXELS_X-1 )Serial.print(",");
    }
    Serial.println();
  }
  Serial.println();
}
*/

//-------------------------------------------------------------------------------------------------------------------------
// j - show x,y and squal values constantly until user presses a key
// added if 'w' reset totals else exit
//-------------------------------------------------------------------------------------------------------------------------
void display_motion()
{
  boolean first_time = true;
  int value;      //added JCH
  int tot_dx;     //added JCH
  int tot_dy;     //added JCH
  boolean brkout = false;
  
//  Serial.flush();  //flush def chged.  myFlush empties buffer.
  myFlush();

  // display instructions on how to exit
  Serial.println("press x to return to menu..");
  delay(1000);
	
      tot_dx = 10;
      tot_dy = 10;

  while( true ) {
    if ( Serial.available() ) {
      delay( 20 );
      value = Serial.read();
      if ( value == 'w' ) {
        tot_dx = 0;
        tot_dy = 0;
        myFlush();
      }else{
        brkout = true;
      }
    }

    if ( brkout ) {
      Serial.println("break");
      break;    //stop, exit while
    }
    
    updateOF();             //proceed get more data

    // check for errors
    if( _overflow ) Serial.println("overflow!!");

    // totalize x & y values
    tot_dx += raw_dx;
    tot_dy += raw_dy;
//    tot_dx += 1;
//    tot_dy += 1;

    // print - x,y,squal,tot x,tot y
    Serial.print("dx: ");
    Serial.print(raw_dx,DEC);
    Serial.print("\tdy: ");
    Serial.print(raw_dy,DEC);
    Serial.print("\tsqual:");
    Serial.print(surface_quality,DEC);
    Serial.print("\t\ttotx:");
    Serial.print(tot_dx,DEC);
    Serial.print("\ttoty:");
    Serial.print(tot_dy,DEC);
    Serial.print("\t\ttotftx:");
    Serial.print(tot_dx/510.0,3);
    Serial.print("\ttotfty:");
    Serial.print(tot_dy/535.0,3);
    Serial.println();
    first_time = false;
		
    // short delay
    delay(100);
	
  // flush the serial
//  Serial.flush();  //flush def chged.  myFlush empties buffer.
  }
    myFlush();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADNS3080 SPECIFIC FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------------
// reset sensor by holding a pin high (or is it low?) for 10us.
//-------------------------------------------------------------------------------------------------------------------------
void reset()
{
  // return immediately if the reset pin is not defined
  if( _reset_pin == 0)
    return;

  digitalWrite(_reset_pin,HIGH);                 // reset sensor
  delayMicroseconds(10);
  digitalWrite(_reset_pin,LOW);                  // return sensor to normal
}


//-------------------------------------------------------------------------------------------------------------------------
// Read a register from the sensor
//-------------------------------------------------------------------------------------------------------------------------
byte read_register(byte address)
{
  byte result = 0, junk = 0;

  backup_spi_settings();

  // take the chip select low to select the device
  digitalWrite(_cs_pin, LOW);

  // send the device the register you want to read:
  junk = SPI.transfer(address);

  // small delay
  delayMicroseconds(50);

  // send a value of 0 to read the first byte returned:
  result = SPI.transfer(0x00);

  // take the chip select high to de-select:
  digitalWrite(_cs_pin, HIGH);

  restore_spi_settings();

  return result;
}


//-------------------------------------------------------------------------------------------------------------------------
// init - initialise sensor
// initCommAPI parameter controls whether SPI interface is initialised (set to false if other devices are on the SPI bus and have already initialised the interface)
//-------------------------------------------------------------------------------------------------------------------------
boolean initOF()
{
  int retry = 0;
  boolean retVal=false;
  int saveVal;

  pinMode(AP_SPI_DATAOUT,OUTPUT);
  pinMode(AP_SPI_DATAIN,INPUT);
  pinMode(AP_SPI_CLOCK,OUTPUT);
  pinMode(_cs_pin,OUTPUT);
  if( _reset_pin != 0)
    pinMode(ADNS3080_RESET,OUTPUT);

  digitalWrite(_cs_pin,HIGH);                 // disable device (Chip select is active low)

  // reset the device
  reset();

  // start the SPI library:
  SPI.begin();

  // check the sensor is functioning
  if( retry < 3 && !retVal) {
    saveVal = read_register(ADNS3080_PRODUCT_ID);
    if( read_register(ADNS3080_PRODUCT_ID) == 0x17 )
      retVal=true;
    retry++;
  }
  Serial.print(saveVal);
  Serial.print(", ");
  Serial.print(_cs_pin);
  Serial.println();
  digitalWrite(_cs_pin,HIGH);                 // disable device (Chip select is active low)
  return retVal;
}


//-------------------------------------------------------------------------------------------------------------------------
// backup_spi_settings - checks current SPI settings (clock speed, etc), sets values to what we need
//-------------------------------------------------------------------------------------------------------------------------
byte backup_spi_settings()
{
  // store current spi values
  orig_spi_settings_spcr = SPCR & (DORD | CPOL | CPHA);
  orig_spi_settings_spsr = SPSR & SPI2X;
  
  // set the values that we need
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // sensor running at 2Mhz.  this is it's maximum speed
  
  return orig_spi_settings_spcr;
}


//-------------------------------------------------------------------------------------------------------------------------
// restore_spi_settings - restores SPI settings (clock speed, etc) to what their values were before the sensor used the bus
//-------------------------------------------------------------------------------------------------------------------------
byte restore_spi_settings()
{
  byte temp;

  // restore SPSR
  temp = SPSR;
  temp &= ~SPI2X;
  temp |= orig_spi_settings_spsr;
  SPSR = temp;

  // restore SPCR
  temp = SPCR;
  temp &= ~(DORD | CPOL | CPHA);   // zero out the important bits
  temp |= orig_spi_settings_spcr;  // restore important bits
  SPCR = temp;

  return temp;
}


//-------------------------------------------------------------------------------------------------------------------------
// write a value to one of the sensor's registers
//-------------------------------------------------------------------------------------------------------------------------
void write_register(byte address, byte value)
{
  byte junk = 0;

  backup_spi_settings();

  // take the chip select low to select the device
  digitalWrite(_cs_pin, LOW);

  // send register address
  junk = SPI.transfer(address | 0x80 );

  // small delay
  delayMicroseconds(50);

  // send data
  junk = SPI.transfer(value);

  // take the chip select high to de-select:
  digitalWrite(_cs_pin, HIGH);

  restore_spi_settings();
}


//-------------------------------------------------------------------------------------------------------------------------
// get_pixel_data - captures an image from the sensor and stores it to the pixe_data array
//-------------------------------------------------------------------------------------------------------------------------
void print_pixel_data(Stream *serPort)
{
  int i,j;
  boolean isFirstPixel = true;
  byte regValue;
  byte pixelValue;

  // write to frame capture register to force capture of frame
  write_register(ADNS3080_FRAME_CAPTURE,0x83);

  // wait 3 frame periods + 10 nanoseconds for frame to be captured
  delayMicroseconds(1510);  // min frame speed is 2000 frames/second so 1 frame = 500 nano seconds.  so 500 x 3 + 10 = 1510

  // display the pixel data
  for( i=0; i<ADNS3080_PIXELS_Y; i++ ) {
    for( j=0; j<ADNS3080_PIXELS_X; j++ ) {
      regValue = read_register(ADNS3080_FRAME_CAPTURE);
      if( isFirstPixel && (regValue & 0x40) == 0 ) {
        serPort->println("failed to find first pixel");
      }
      isFirstPixel = false;
      pixelValue = ( regValue << 2);
      serPort->print(pixelValue,DEC);
//      pixelVal1[(i*30) + j]= pixelValue;
//      pixelVal1[i][j]= pixelValue;
      if( j!= ADNS3080_PIXELS_X-1 )
        serPort->print(",");
      delayMicroseconds(50);
    }
    serPort->println();
  }

  // hardware reset to restore sensor to normal operation
  reset();
}


/* -------------------------------------------------------------------------------------------------------------------------
// get_pixel_data - captures an image from the sensor and stores it to the pixel_data array.  Does not print it.
//-------------------------------------------------------------------------------------------------------------------------
//void get_pixel_data(Stream *serPort)
void get_pixel_data()
{
  int i,j;
  boolean isFirstPixel = true;
  byte regValue;
  byte pixelValue;

  // write to frame capture register to force capture of frame
  write_register(ADNS3080_FRAME_CAPTURE,0x83);

  // wait 3 frame periods + 10 nanoseconds for frame to be captured
  delayMicroseconds(1510);  // min frame speed is 2000 frames/second so 1 frame = 500 nano seconds.  so 500 x 3 + 10 = 1510

  // display the pixel data
  for( i=0; i<ADNS3080_PIXELS_Y; i++ ) {
    for( j=0; j<ADNS3080_PIXELS_X; j++ ) {
      regValue = read_register(ADNS3080_FRAME_CAPTURE);
//      if( isFirstPixel && (regValue & 0x40) == 0 ) {
//        serPort->println("failed to find first pixel");
//      }
      isFirstPixel = false;
      pixelValue = ( regValue << 2);
//      serPort->print(pixelValue,DEC);
//      pixelVal1[(i*30) + j]= pixelValue;
      pixelVal1[i][j]= pixelValue;
//      if( j!= ADNS3080_PIXELS_X-1 )
//        serPort->print(",");
      delayMicroseconds(50);
    }
//    serPort->println();
  }

  // hardware reset to restore sensor to normal operation
  reset();
}
*/
//-------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------
bool updateOF()
{
  byte motion_reg;
  surface_quality = (unsigned int)read_register(ADNS3080_SQUAL);
  delayMicroseconds(50);  // small delay

  // check for movement, update x,y values
  motion_reg = read_register(ADNS3080_MOTION);
// was testing  motion_reg = true;
  _overflow = ((motion_reg & 0x10) != 0);  // check if we've had an overflow
  if( (motion_reg & 0x80) != 0 ) {
    raw_dx = ((char)read_register(ADNS3080_DELTA_X));
    delayMicroseconds(50);  // small delay
    raw_dy = ((char)read_register(ADNS3080_DELTA_Y));
    _motion = true;
  }else{
    raw_dx = 0;
    raw_dy = 0;
  }
  last_update = millis();

  return true;
}


//-------------------------------------------------------------------------------------------------------------------------
// Chg def of Serial.flush.  This is my fix.
//-------------------------------------------------------------------------------------------------------------------------
void myFlush(){

  while (Serial.available()) {
    Serial.read(); 
  }
	
  
}


//-------------------------------------------------------------------------------------------------------------------------
// get registers 0 thru 10, c
//-------------------------------------------------------------------------------------------------------------------------
void display_config()
{
  int i;
  boolean isFirstPixel = true;
  byte regValue;
  byte pixelValue;

  Serial.print(read_register(ADNS3080_PRODUCT_ID), DEC);         //  0x00
  Serial.println();
  Serial.print(read_register(ADNS3080_REVISION_ID), DEC);        //  0x01
  Serial.println();
  Serial.print(read_register(ADNS3080_MOTION), DEC);             //  0x02
  Serial.println();
  Serial.print(read_register(ADNS3080_DELTA_X), DEC);            //  0x03
  Serial.println();
  Serial.print(read_register(ADNS3080_DELTA_Y), DEC);            //  0x04
  Serial.println();
  Serial.print(read_register(ADNS3080_SQUAL), DEC);              //  0x05
  Serial.println();
  Serial.print(read_register(ADNS3080_PIXEL_SUM), DEC);          //  0x06
  Serial.println();
  Serial.print(read_register(ADNS3080_MAXIMUM_PIXEL), DEC);      //  0x07
  Serial.println();
  Serial.print(read_register(ADNS3080_CONFIGURATION_BITS), DEC); //  0x0a
  Serial.println();
  Serial.println();

  // hardware reset to restore sensor to normal operation
  reset();
  delay(1000);
}


//-------------------------------------------------------------------------------------------------------------------------
// Read a register from the sensor
//-------------------------------------------------------------------------------------------------------------------------
int read_register2(byte address)
{
  byte result = 0, junk = 0;

  backup_spi_settings();

  // take the chip select low to select the device
  digitalWrite(_cs_pin, LOW);

  // send the device the register you want to read:
  junk = SPI.transfer(address);

  // small delay
  delayMicroseconds(50);

  // send a value of 0 to read the first byte returned:
  result = SPI.transfer(0x00);

  // take the chip select high to de-select:
  digitalWrite(_cs_pin, HIGH);

  restore_spi_settings();

  return result;
}

