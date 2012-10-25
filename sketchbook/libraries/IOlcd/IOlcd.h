#ifndef IOlcd_h
#define IOlcd_h

#include <Wire.h>;
#include <inttypes.h>
#include "Arduino.h"
#include "Print.h"

//#define address 0x20
#define Enable 64
#define RS 128
#define IODIR0 0x06
#define IODIR1 0x07

#define allHigh 0xFF
#define allLow 0x00
#define allInput 0xFF
#define allOutput 0x00
#define Port0 0x00
#define Port1 0x01

class IOlcd : public Print{
	int _address;
	public:
		void begin(uint8_t rows, uint8_t collumsn, int _address);
		void init(int _address);
		void clear();
		void blinkCursor();
		void noBlinkCursor();
		void home();
		void scrollDisplayLeft();
		void scrollDisplayRight();
		void setCursor(uint8_t x, uint8_t y);
		void createChar(uint8_t location, uint8_t charmap[]);
		virtual size_t write(uint8_t c);
	private:
		int byteShift(int num);
		void writeCommand(uint8_t _data);
		void sendData(uint8_t port, uint8_t _data);
	};
#endif
