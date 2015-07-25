#include "stripcontrol.h"

int stripselect;

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
