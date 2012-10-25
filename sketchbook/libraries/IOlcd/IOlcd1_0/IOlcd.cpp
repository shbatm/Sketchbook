#include <IOlcd.h>

#include <Wire.h>
#include "Arduino.h"

//constructors//
IOlcd::IOlcd()
{
}

//public methods //

bool IOlcd::init(uint8_t address, uint8_t device_type)
{
	switch(device_type){
		case 1:
			REGISTER_GP0 = 0x00;
			REGISTER_IODIR0 = 0x06;
			break;
		case 2 ... 3:
			REGISTER_GP0 = 0x12;
			REGISTER_IODIR0 = 0x00;
			break;
		default:
			return false;
	}
	
	addr = address;
	port0 = ALL_LOW;
	port1 = ALL_LOW;
	regPort0 = ALL_INPUT;
	regPort1 = ALL_INPUT;
	
	Wire.begin();
	
	pinModePort(0,INPUT);
	pinModePort(0,INPUT);
	return refresh();
}


bool IOlcd::pinMode(uint8_t port,uint8_t pin, bool mode)
{
	if(port > 1 || pin > 7) return false;
	uint8_t temp = 0;
	
	if(!mode){
		//pin setup as input = 1
		temp = 1 << pin;
		if(port == 0)
			regPort0 |= temp;
		else
			regPort1 |= temp;
	}		
	else{
		//pin setup as output = 0
		temp = 1 << pin;
		temp = ~temp;
		if(port == 0)
			regPort0 &= temp;
		else
			regPort1 &= temp;
	}
	
	//send new config to IODIR0 and 1
	Wire.beginTransmission(addr);		//start transmission
	Wire.write(REGISTER_IODIR0);			//select register IODIR0
	Wire.write(regPort0);				//DDR Port0 IODIRO
	Wire.write(regPort1);				//DDR Port1 IODIR1
	if(Wire.endTransmission() == 0)
		return true;
	else
		return false;
}


bool IOlcd::pinModePort(uint8_t port, bool mode)
{
	if(port > 1) return false; // we only have two ports
	
	if(!mode){
	//DDR as input = 1
		if(port == 0)
			regPort0 = ALL_INPUT;
		else
			regPort1 = ALL_INPUT;
	}
	else{
	//DDr as output = 0
		if(port == 0)
			regPort0 = ALL_OUTPUT;
		else
			regPort1 = ALL_OUTPUT;
	}
	
	//send new config to IODIR REGISTER
	Wire.beginTransmission(addr);		//start transmission
	Wire.write(REGISTER_IODIR0);			//select REGISTER_IODIR0
	Wire.write(regPort0);				//DDR port0 IODIR0
	Wire.write(regPort1);				//DDR port1 IODIR1
	if(Wire.endTransmission() == 0)
		return true;
	else
		return false;
}

bool IOlcd::sendData()
{
	Wire.beginTransmission(addr);
	Wire.write(REGISTER_GP0);	//select general purpose register
	Wire.write(port0);			//write to REGISTER_GP0	
	Wire.write(port1);			//write to REGISTER_GP1
	if(Wire.endTransmission() == 0)
		return true;
	else
		return false;
}


bool IOlcd::refresh()
{
  Wire.beginTransmission(addr);
  Wire.write(REGISTER_GP0); //select General purpose register
  if(Wire.endTransmission() > 0) return false;
  
  Wire.requestFrom(addr, (uint8_t)2); //request two bytes, register gp1 and 1-2
    if(Wire.available() == 2){
		port0 = Wire.read();  //recv REGISTER_GP0
		port1 = Wire.read();  //recv REGISTER_GP1
		return true;
	}else
		return false;
}

void IOlcd::begin(uint8_t cols, uint8_t lines){
	//tell the ic that port0 all output and port1 pin 6 and 7 output
	pinMode(1,6, OUTPUT);
	pinMode(1,7, OUTPUT);
	pinModePort(0, OUTPUT);
	
	//tell we are bitmode 8 and 2 lines
	writeCommand(0x38);
	//wait a bit
	delayMicroseconds(4500);
	//try second time
	writeCommand(0x38);
	delayMicroseconds(1500);
	//third and last try
	writeCommand(0x38);
	
	//set display on and enter entry mode
	writeCommand(0x0c);
	writeCommand(0x06);
	writeCommand(0x02);
}

void IOlcd::clear(){
	writeCommand(0x01);
}

void IOlcd::blinkCursor(){
	writeCommand(0x0f);
}

void IOlcd::noBlinkCursor(){
	writeCommand(0x0c);
}

void IOlcd::home(){
	writeCommand(0x02);
}

void IOlcd::scrollDisplayLeft(){
	writeCommand(0x18);
}

void IOlcd::scrollDisplayRight(){
	writeCommand(0x1c);
}

void IOlcd::setCursor(uint8_t x, uint8_t y){
	if(y == 0){
		home();
		writeCommand(0x80+x);
	}
	if(y == 1){
		home();
		writeCommand(0x80+0x40+x);
	}
}

void IOlcd::createChar(uint8_t location, uint8_t charmap[]){
	location &= 0x7;
	writeCommand(0x40 | (location << 3));
	for(int i=0; i<8;i++){
		write(charmap[i]);
	}
}

inline size_t IOlcd::write(uint8_t c){
	c = byteShift(c);
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x00);
	Wire.write((uint8_t)c);
	Wire.endTransmission();
	
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x01);
	Wire.write((uint8_t)B11000000);
	Wire.endTransmission();
	
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x01);
	Wire.write((uint8_t)B10000000);
	Wire.endTransmission();
	
	delayMicroseconds(100);
	
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x01);
	Wire.write(B01000000);
	Wire.endTransmission();
}

void IOlcd::writeCommand(uint8_t command){
	command = byteShift(command);
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x00);
	Wire.write((uint8_t)command);
	Wire.endTransmission();
	
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x01);
	Wire.write((uint8_t)B01000000); //write pin E HIGH
	Wire.endTransmission();
	
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x01);
	Wire.write((uint8_t)B00000000); //write pin E LOW
	Wire.endTransmission();  
	
	delayMicroseconds(100);
	
	Wire.beginTransmission(addr);
	Wire.write((uint8_t)0x01);
	Wire.write((uint8_t)B01000000); //write pin E HIGH
	Wire.endTransmission();
	
	delayMicroseconds(40);
}

//this is not my code,
//borowed from here: 
//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1256179312
int IOlcd::byteShift(int num) {
  int var = 0;    
  int i, x, y, p;
  int s = 8;    // number of bits in 'num'. (This case a 4bit nibble)

  for (i = 0; i < (s / 2); i++) {
    // extract bit on the left, from MSB
    p = s - i - 1;
    x = num & (1 << p);
    x = x >> p;  
    // extract bit on the right, from LSB
    y = num & (1 << i);
    y = y >> i;
  
    var = var | (x << i);       // apply x
    var = var | (y << p);       // apply y
  }
  return var;
}

