#include "analogLedStrip.h"
#include "stripcontrol.h"

int colors[3] = {0, 0, 0};
int value = 0;
int pledselect = 0;
int ledselect = 0;

unsigned long ccurrent = 0;
unsigned long cprevious = 0;
int cinterval = 10;

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

void colorinc()
{
    value++;
    if(value > 255)
    {
        ledselect++;
        value = 1;
        if(ledselect == 3)
        {
            ledselect = 0;
            pledselect = 2;
        }
        else
        {
            pledselect = ledselect - 1;
        }
    }
    colors[pledselect] = 255 - value;
    colors[ledselect] = value;
}

void fadeRgb(int speed, int brightness)
{
    ccurrent = millis();
    if((ccurrent - cprevious) >= cinterval)
    {
        cprevious = ccurrent;
        colorinc();
    }
    cinterval = stripcontrol.varZero+1;
    float brightnessFactor = (float)(((float)stripcontrol.brightness)/100);
    int r = colors[RED] * brightnessFactor;
    int g = colors[GREEN] * brightnessFactor;
    int b = colors[BLUE] * brightnessFactor;
    writeRgb(r, g, b);
}