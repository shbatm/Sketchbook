#include <IOlcd.h>
#include <Wire.h>;
#include "Arduino.h"


void IOlcd::init(int _address){
	delay(100);
	Wire.begin(_address);
	sendData(IODIR0, allOutput);
	sendData(IODIR1, allOutput);
	}

void IOlcd::begin(uint8_t rows, uint8_t collumns, int address){
	_address = address;
	init(_address);
	delay(100);
	writeCommand(0x30);
	delay(5);
	writeCommand(0x30);
	delayMicroseconds(0x30);
	writeCommand(0x38);
	delayMicroseconds(60);
	writeCommand(0x08);
	delayMicroseconds(60);
	writeCommand(0x01);
	delay(5);
	writeCommand(0x06);
	delayMicroseconds(60);
	writeCommand(0x0C);
	delayMicroseconds(60);
	}

int IOlcd::byteShift(int num){
	int var = 0;
	int i, x, y, p;
	int s = 8;
	
	for(i=0;i<(s/2);i++){
		p = s-i-1;
		x = num&(1<<p);
		x = x >> p;
		y = num & (1<<i);
		y = y>>i;
		var = var | (x<<i);
		var = var | (y<<p);
		}
	return var;
	}

void IOlcd::clear(){
	writeCommand(0x01);
	}

void IOlcd::blinkCursor(){
	writeCommand(0x0F);
	}

void IOlcd::noBlinkCursor(){
	writeCommand(0x0C);
	}

void IOlcd::home(){
	writeCommand(0x02);
	}

void IOlcd::scrollDisplayLeft(){
	writeCommand(0x18);
	}

void IOlcd::scrollDisplayRight(){
	writeCommand(0x1C);
	}

void IOlcd::setCursor(uint8_t x, uint8_t y){
	int row_offsets[] = {0x00,0x40,0x14,0x54};
	if(y==0){
		home();
		writeCommand(0x80+row_offsets[y]+x);
		}
	if(y==1){
		home();
		writeCommand(0x80+row_offsets[y]+x);
		}
	if(y==2){
		home();
		writeCommand(0x80+row_offsets[y]+x);
		}
	if(y==3){
		home();
		writeCommand(0x80+row_offsets[y]+x);
		}
	}

void IOlcd::createChar(uint8_t location, uint8_t charmap[]){
	location &= 0x70;
	writeCommand(0x40|(location<<3));
	for(int i=0;i<8;i++){
		write(charmap[i]);
		}
	}

void IOlcd::writeCommand(uint8_t _data){
	_data = byteShift(_data);
	sendData(Port0, _data);
	sendData(Port1, Enable);
	sendData(Port1, 0x00);//write enable low;
	delayMicroseconds(60);
	}

inline size_t IOlcd::write(uint8_t _data){
	_data = byteShift(_data);
	sendData(Port0, _data);
	sendData(Port1, RS|Enable);
	sendData(Port1, 0x00);
	delayMicroseconds(60);
	}

void IOlcd::sendData(uint8_t port, uint8_t _data){
	Wire.beginTransmission(_address);
	Wire.write(port);
	Wire.write(_data);
	Wire.endTransmission();
	}
