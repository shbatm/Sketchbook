/*
 * the registers determine what the port will be set to,
 * either a input or output.
 * GP0 is register for port0 address on 0x06
 * GP1 is register for port1 address on 0x07
 * sending a 0 will set the relative pin to output.
 * sending a 1 will set the relative pin to input.
 * like pins: 			 7,6,5,4,3,2,1,0
 * corresponds to bit:  B1 0 1 0 1 0 1 0
 * 
 * to talk to the pins, you first send what port you want to talk too.
 * port0 is 0x00 and port port1 is 0x01;
 * 
 * say you set pins on both port0 and port1 to OUTPUT:
 * for port0 you send first 0x06 then 0x00
 * for port1 you send first 0x07 then 0x00 
 * (0xFF will set them all to output (B00000000);
 * (0x00 will set them all to INPUT (B11111111);
 * if want to make pin 1 on port0 high you send first 0x00 and then 0x01;
 * and if you want to make pin 3 on port1 high you make send 0x01 first and then
 * you send 0x04. (B00000100)
 * 
 * If i read it right, in the recourses there is not much to do,
 * To initilize the Glcd, other then let it wait a bit for it to initialize.
 * 
 * Ok I arranged it like this on the ports:
 * I use port0 as the Data Buss, connecting the data lines to it.
 * Data line 1 goes to pin 0 (first pin on port0).
 * 
 * and for port1 I use that for the instruction set, reading writing, what
 * side of the display to use.
 * thing I am going to do that like:
 * 
 * pin 1: Chip Select 1.
 * pin 2: Chip Select 2.
 * pin 3: RS.
 * pin 4: R/W.
 * pin 5: Reset.
 * pin 6: Enable.
 * pin 7: N/C
 * pin 8: N/C
 * 
 * so the send byte's are going to be as follows:
 * 
 * GP0 				= 0x06;
 * GP1 				= 0x07;
 * Port0 			= 0x00;
 * Port1 			= 0x01;
 * 
 * ChipSelect 1 	= 0x01; first pin
 * ChipSelect 2 	= 0x02; second pin
 * RS 				= 0x04; third pin
 * R/W 				= 0x08; fourth pin
 * Reset 			= 0x10; fifth pin
 * Enable 			= 0x20; sixth pin
 * 
 * a L is low = 0;
 * a H is high = 1;
 * 
 * the datasheet says:
 * for a instruction:
 * RS = L and R/W = L
 * 
 * for status read busy flag:
 * RS = L and R/W = H
 * 
 * for Data write:
 * RS = H and R/W = L
 * 
 * for Data Read:
 * RS=H and R/W = H
 * 
 * 
 * */
 
#include <Wire.h>

#define GP0 0x06
#define GP1 0x07
#define Port0 0x00
#define Port1 0x01

#define allInput 0xFF
#define allOutput 0x00

#define allLow 0x00
#define allHigh 0xFF

#define ChipSelect1 0x01
#define ChipSelect2 0x02
#define RS 0x04
#define R_W 0x08
#define Reset 0x10
#define Enable 0x20

#define address 0x20
byte instructionSet = 0;
byte chipselect = 0;

void setup(){
	Wire.begin(address);
	//why not just make everything Output for now :)
	writeData(GP0, allOutput);
	writeData(GP1, allOutput);
	//lets try to first reset the device by sending reset and enable.
	//delay(3000);
	delay(10);
	writeData(Port1, LOW);
	writeData(Port1, Reset|Enable);
	delay(1);
	chipselect = ChipSelect1;
	writeCmd(0x3F);
	chipselect = ChipSelect2;
	writeCmd(0x3F);
	//writeData(Port1, 0x00);
	//writeData(Port1, ChipSelect1);
	//writeData(Port1, ChipSelect2);
	}
	
void loop(){
	
	}

void writeCmd(byte data){
	writeData(Port1, Reset|Enable|chipselect);
	writeData(Port0, data);
	writeData(Port1, Reset|chipselect);
	}

void writeData(uint8_t register_, uint8_t port_){
	Wire.beginTransmission(address);
	Wire.write(register_);
	Wire.write(port_);
	Wire.endTransmission();
	}
