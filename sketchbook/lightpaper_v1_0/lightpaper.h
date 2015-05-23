#ifndef LIGHTPAPER_H
#define LIGHTPAPER_H

#include <stdlib.h>
#include <Arduino.h>
#include <font5x7.h>

#define SCLK 2     // purple wire
#define R 4        // green wire

#define A 6         // orange wire
#define B 14         // yellow wire
#define C 15        // brown wire

void setupRows();
void setupClock();
void setupLightPaper();
void flashrow(uint8_t);
void showrow(uint8_t, uint8_t);
void clock();
void clearRow();
void setCol(uint8_t);
void writeCol(uint8_t, uint8_t);
void setRowData(uint8_t);
void putStr(char *);
unsigned char reverse(unsigned char b);
void putChar(char, uint8_t);
void putChar(char);
void writeout(uint8_t);
void writeout();
void setLPBuffer(uint8_t);
void marquee(char *);
void showuptime();
void clearLPBuffer();
void clearLPStrBuffer();
char *getLPStrBuffer();
uint8_t *getLPBuffer();
uint8_t getLPNumLetters();
uint8_t getLPHeight();
uint8_t getLPWidth();
uint8_t getLPMaxLetterWidth();
uint8_t getLPBufferSize();

#endif