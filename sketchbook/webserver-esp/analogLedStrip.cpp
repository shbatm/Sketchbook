#include "analogLedStrip.h"

void setupAnalogStrip()
{
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    analogWrite(REDPIN, 0);
    analogWrite(GREENPIN, 0);
    analogWrite(BLUEPIN, 0);
}

void writeRgb(int rval, int gval, int bval)
{
    analogWrite(REDPIN, rval);
    analogWrite(GREENPIN, gval);
    analogWrite(BLUEPIN, bval);
}