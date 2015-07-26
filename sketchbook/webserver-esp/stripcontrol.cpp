#include "stripcontrol.h"

int stripselect;
int colors[3] = {0, 0, 0};
int value = 0;
int pledselect = 0;
int ledselect = 0;

unsigned long ccurrent = 0;
unsigned long cprevious = 0;
int cinterval = 10;

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
    // char fmtstr[100];
    // sprintf(fmtstr,
    //         "\n"
    //         "pledselect: %d\n"
    //         "ledselect:  %d\n"
    //         "value:      %d\n"
    //         "255 - value:%d\n",
    //         pledselect,
    //         ledselect,
    //         value,
    //         255-value);
    // Serial.println(fmtstr);
}

void setupStrips()
{
    setupAnalogStrip();
    stripselect = ANALOGSTRIP;
}

void handleStrips()
{
    if(stripcontrol.effect == RGBCOLORS)
    {
        if(stripselect == ANALOGSTRIP)
        {
            int r = stripcontrol.varZero;
            int g = stripcontrol.varOne;
            int b = stripcontrol.varTwo;
            writeRgb(r, g, b);
        }
        else if(stripselect == WS2801);
        else if(stripselect == WS2812);
    }
    else if(stripcontrol.effect == FADING)
    {
        if(stripselect == ANALOGSTRIP)
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
        else if(stripselect == WS2801);
        else if(stripselect == WS2812);
    }
    else if(stripcontrol.effect == DIGITALFADING)
    {

    }
}

void debugPrintStripControl()
{
    char fmtstr[100];
    sprintf(fmtstr, 
        "\nDebug:\n"
        "pincode:     %d\n"
        "effect:      %d\n"
        "brightness:  %d\n"
        "var0:        %d\n"
        "var1:        %d\n"
        "var2:        %d\n"
        "changed:     %d\n",
        stripcontrol.pincode,
        stripcontrol.effect,
        stripcontrol.brightness,
        stripcontrol.varZero,
        stripcontrol.varOne,
        stripcontrol.varTwo,
        stripcontrol.changed
        );
    Serial.println(fmtstr);
}
