#ifndef LIGHTPAPER_H
#define LIGHTPAPER_H

#include <stdlib.h>
#include <Arduino.h>
#include "font5x7.h"

#define SCLK 2     // purple wire
#define R 4        // green wire

#define A 6         // orange wire
#define B 14        // yellow wire
#define C 15        // brown wire

// selects a row to display
void setupRows();
// sets up the clock pin
void setupClock();
// sets up  all the pins
void setupLightPaper();
// flashes a row to be shown.
void flashrow(uint8_t);
// shows a single row of data.
void showrow(uint8_t, uint8_t);
// togles the clock pin
void clock();
// clears data on a row.
void clearRow();
// sets a collumn on pixels to be shown.
void setCol(uint8_t);
// writes data to a collumn of pixels.
void writeCol(uint8_t, uint8_t);
// displays data on a row.
void setRowData(uint8_t);
// puts a string on the ledmatrix
void putStr(char *);
// reverse a single byte of data.
unsigned char reverse(unsigned char b);
// puts a character onto the display at a offset.
void putChar(char, uint8_t);
// put a character onto the display.
void putChar(char);
// writes out data to display. with offset.
void writeout(uint8_t);
// writes out data to display in buffer.
void writeout();
// fill buffer with data.
void setLPBuffer(uint8_t);
// marquee some text point by char *
void marquee(char *);
// displays running time on display.
void showuptime();
// clears the buffer and thus the display.
void clearLPBuffer();
// clears the string buffer.
void clearLPStrBuffer();
// returns a reference to the string buffer.
char *getLPStrBuffer();
// returns a reference to the byte buffer.
uint8_t *getLPBuffer();
// returns the number of letters fit in string buffer.
uint8_t getLPNumLetters();
// returns the height of display.
uint8_t getLPHeight();
// returs the number of pixels the display is wide 
uint8_t getLPWidth();
// returns the width of a single character.
uint8_t getLPMaxLetterWidth();
// returns the size of the pixel buffer.
const uint8_t getLPBufferSize();
// write directly into pixel buffer.
void writeLPBuffer(uint8_t *, int);

#endif
