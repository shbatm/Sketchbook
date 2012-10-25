#ifndef Xmem_h
#define Xmem_h
//#include "WProgram.h"
#include "Arduino.h"

class Xmem{
	public:
		Xmem(int latchpin, int clockpin, int datapin);
		//public function.
		uint8_t read(unsigned int address);
		void write(uint8_t Data, unsigned int address);
	private:		
		//pins needed for writing address lines with the shiftRegisters.
		uint8_t _latchpin;
		uint8_t _clockpin;
		uint8_t _datapin; 
		void writeOut(unsigned int address);
	};
#endif
