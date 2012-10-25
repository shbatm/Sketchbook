
//#include "WProgram.h"
#include <Xmem.h>
#include "Arduino.h"


#define CLEAROUTPUTENABLE (PORTC &= ~0x10)
#define SETOUTPUTENABLE   (PORTC |= 0x10)
#define CLEARWRITEENABLE  (PORTC &= ~0x20)
#define SETWRITEENABLE    (PORTC |= 0x20)
#define SETWRITEENABLEOUTPUT (DDRC |= 0x10)
#define SETOUTPUTENABLEOUTPUT (DDRC |= 0x20)

//Constructor.
Xmem::Xmem(int latchpin, int clockpin, int datapin){//, int outputEnable, int writeEnable){
	_latchpin = latchpin;
	_clockpin = clockpin;
	_datapin = datapin;
	pinMode(_clockpin, OUTPUT);
	pinMode(_datapin, OUTPUT);
	pinMode(_latchpin, OUTPUT);
	SETWRITEENABLEOUTPUT;
	SETOUTPUTENABLEOUTPUT;
	}

void Xmem::writeOut(unsigned int address){
	digitalWrite(_latchpin, LOW); 
	shiftOut(_datapin, _clockpin, MSBFIRST, address>>8); // Most left 8 bits transferred
	shiftOut(_datapin, _clockpin, MSBFIRST, address&255); // Most right 8 bits transferred
	digitalWrite(_latchpin, HIGH);
	}

uint8_t Xmem::read(unsigned int address){
	//we pull readRamEnable, LOW because we want to read.
	//digitalWrite(_outputEnable, LOW);
	CLEAROUTPUTENABLE;
	//we make a empty byte to write in.
	uint8_t Data = 0;
	writeOut(address);
	//we set the pins we read with to input.
	DDRD = 0b00000010; //a zero is a input, the reason for the 1 is serial.
	DDRB = 0b11111100;//0xFC; //on portb we set the first two pins to input and mask the rest.
	Data = ((PIND & 0xFC)>>2) | ((PINB & 0x03)<<6);
	//we set readRamEnable HIGH since we are done with reading.
	SETOUTPUTENABLE;
	//and return our data.
	return Data;
	}

void Xmem::write(uint8_t Data, unsigned int address){
	//we write writeRamEnable, LOW as to we want to write to the ram.
	CLEARWRITEENABLE;
	//digitalWrite(_writeEnable, LOW);
	//we ask from what address we want to read.
	writeOut(address);
	//we set the pins we need to output a 1 is a output.
	DDRD = DDRD | 0b11111110;
	DDRB = DDRB | 0b00000011;//0x03;
	//we write the data to the i/o pins.
	PORTD = Data<<2;
	PORTB = Data>>6;
	//we write writeRamEnable, high since we are done wiring.
	//digitalWrite(_writeEnable, HIGH);
	SETWRITEENABLE;
	}
