#include "ws2801.h"
#include "stripcontrol.h"

static uint8_t *buffer[3] = {NULL, NULL, NULL};

static int striplen;

static unsigned long ccurrent = 0;
static unsigned long cprevious = 0;
static int cinterval = 10;

static uint8_t *colors;

void setupWS2801(int freq, int len)
{
    SPI.begin();
    SPI.setFrequency(1e6);
    striplen = len;
    colors = colorinc();
    // allocate space if run first time else free and allocate.
    if(buffer[0] == NULL && buffer[1] == NULL && buffer[2] == NULL)
    {
        // allocate space for buffer
        buffer[0] = (uint8_t *)malloc(sizeof(uint8_t)*len);
        buffer[1] = (uint8_t *)malloc(sizeof(uint8_t)*len);
        buffer[2] = (uint8_t *)malloc(sizeof(uint8_t)*len);
    }
    else
    {
        free(buffer[0]);
        free(buffer[1]);
        free(buffer[2]);
        // allocate space for buffer
        buffer[0] = (uint8_t *)malloc(sizeof(uint8_t)*len);
        buffer[1] = (uint8_t *)malloc(sizeof(uint8_t)*len);
        buffer[2] = (uint8_t *)malloc(sizeof(uint8_t)*len);
    }
}

void setWS2801Pixel(int pos, int r, int g, int b)
{
    buffer[RED][pos] = r;
    buffer[GREEN][pos] = g;
    buffer[BLUE][pos] = b;
}

void setWS2801Strip(int r, int g, int b)
{
    for(int i = 0; i < striplen; i++)
    {
        setWS2801Pixel(i, r, g, b);
    }
}

void fadeWS2801(int speed, int brightness)
{
    ccurrent = millis();
    if((ccurrent - cprevious) >= cinterval)
    {
        cprevious = ccurrent;
        colors = colorinc();
    }

    cinterval = speed+1;
    float brightnessFactor = (float)(((float)brightness)/100);
    int r = colors[RED] * brightnessFactor;
    int g = colors[GREEN] * brightnessFactor;
    int b = colors[BLUE] * brightnessFactor;

    setWS2801Strip(r, g, b);
}

void updateWS2801()
{
    for(int i = 0; i < striplen; i++)
    {
        SPI.transfer(buffer[RED][i]);
        SPI.transfer(buffer[GREEN][i]);
        SPI.transfer(buffer[BLUE][i]);
    }
    delayMicroseconds(500);
}