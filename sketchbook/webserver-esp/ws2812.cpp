#include "ws2812.h"

static int striplen = 0;
NeoPixelBus *strip = NULL;

static int colors[3] = {0, 0, 0};
static int value = 0;
static int pledselect = 0;
static int ledselect = 0;

static unsigned long ccurrent = 0;
static unsigned long cprevious = 0;
static int cinterval = 10;

static void colorinc()
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

void setupWS2812(uint16_t length, uint8_t pin)
{
    striplen = length;
    if(strip == NULL)
    {
        strip = new NeoPixelBus(length, pin);
    }
    else
    {
        delete strip;
        strip = new NeoPixelBus(length, pin);
    }
    strip->Begin();
    strip->Show();
}

void setWS2812Strip(int r, int g, int b)
{
    for(int i = 0; i < striplen;i++)
    {
        RgbColor color = RgbColor(r, g, b);
        strip->SetPixelColor(i, color);
    }
}

void fadeWS2812(int speed, int brightness)
{
    ccurrent = millis();
    if((ccurrent - cprevious) >= cinterval)
    {
        cprevious = ccurrent;
        colorinc();
    }

    cinterval = speed+1;
    float brightnessFactor = (float)(((float)brightness)/100);
    int r = colors[0] * brightnessFactor;
    int g = colors[1] * brightnessFactor;
    int b = colors[2] * brightnessFactor;

    setWS2812Strip(r, g, b);
}

void updateWS2812()
{
    /* bug in strip.show() that doesn't allow this to run acces point mode.*/
    strip->Show();
}