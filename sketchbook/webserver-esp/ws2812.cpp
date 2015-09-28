#include "ws2812.h"
#include "stripcontrol.h"

static int striplen = 0;
NeoPixelBus *strip = NULL;

static unsigned long ccurrent = 0;
static unsigned long cprevious = 0;
static int cinterval = 10;

static uint8_t *colors;

void setupWS2812(uint16_t length, uint8_t pin)
{
    striplen = length;
    colors = colorinc();
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
    int r = colors[RED] * brightnessFactor;
    int g = colors[GREEN] * brightnessFactor;
    int b = colors[BLUE] * brightnessFactor;

    setWS2812Strip(r, g, b);
}

void rainbowWs2812(int speed, int brightness)
{
    cinterval = speed + 1;
    ccurrent = millis();
    if((ccurrent - cprevious) >= cinterval)
    {
        cprevious = ccurrent;
        static int range = 0xff*3;
        uint8_t buffer[3][striplen];
        int i, s;
        uint8_t *color = colorinc();
        for(i = 0; i < striplen; i++)
        {
            for(s = 0; s < range/striplen; s++)
                color = colorinc();
            float brightnessFactor = (float)(((float)brightness) / 100);
            int r = color[RED] * brightnessFactor;
            int g = color[GREEN] * brightnessFactor;
            int b = color[BLUE] * brightnessFactor;
            RgbColor rgbcolor = RgbColor(r, g, b);
            strip->SetPixelColor(i, rgbcolor);
        }
    }
}

void updateWS2812()
{
    strip->Show();
}