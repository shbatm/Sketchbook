#include <Wire.h>;

#define address 0x20
#define Enable 64
#define RS 128

#define IODIR0 0x06
#define IODIR1 0x07

//a High is 1
#define allHigh 0xFF
//a low is 0
#define allLow 0x00
//input is 1
#define allInput 0xFF
//output is 0
#define allOutput 0x00

//Data buss port0
#define Port0 0x00
//Rs and E connected to Port1
#define Port1 0x01


void setup(){
	pinMode(13, OUTPUT);
	begin1();
	for(int i=0;i<255;i++){
		write_(i);
		}
	}

void loop(){
	
	}
//this is what i have in my IOlcd library.
void begin0(){
	initChip();
	delay(100);
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
	delayMicroseconds(60);
	}
	
//this is from here: http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
void begin1(){
	initChip();
	delay(100);
	writeCommand(0x30);
	delay(5);
	writeCommand(0x30);
	delayMicroseconds(150);
	writeCommand(0x30);
	delayMicroseconds(150);
	writeCommand(0x38);
	delayMicroseconds(60);
	writeCommand(0x08);
	delayMicroseconds(60);
	writeCommand(0x01);
	delay(5);
	writeCommand(0x06);
	delayMicroseconds(60);
	//turn the display on:
	writeCommand(0x0C);
	delayMicroseconds(60);
	}
//this is a test init.
void initChip(){
	delay(100); // I read online a little delay for waiting everything is powered up.
	Wire.begin(address);
	sendData(IODIR0, allOutput);
	sendData(IODIR1, allOutput);
	}

int byteShift(int num){
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

//void writeCommand4bits(uint8_t _data){
	//writeCommand(_data>>4);
	//writeCommand(_data&0x08);
	//}

//void write4bits(uint8_t _data){
	//write(_data>>4);
	//write(_data&0x08);
	//}
	
void writeCommand(uint8_t _data){
	_data = byteShift(_data);
	sendData(Port0, _data);
	sendData(Port1, Enable); //write enable high
	sendData(Port1, 0x00); //write enable low;
	delayMicroseconds(60); //write time wait.
	}
void write_(uint8_t _data){
	_data = byteShift(_data);
	sendData(Port0, _data);
	sendData(Port1, RS | Enable);
	sendData(Port1, 0x00);
	delayMicroseconds(60); //write time wait.
	}

void sendData(uint8_t port, uint8_t _data){
	Wire.beginTransmission(address);
	Wire.write(port);
	Wire.write(_data);
	Wire.endTransmission();
	}
