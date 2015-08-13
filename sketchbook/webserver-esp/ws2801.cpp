#include "ws2801.h"

static uint8_t *buffer[3] = {NULL, NULL, NULL};

static int striplen;

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

void setupWS2801(int freq, int len)
{
    SPI.begin();
    SPI.setFrequency(1e6);
    striplen = len;
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
    buffer[0][pos] = r;
    buffer[1][pos] = g;
    buffer[2][pos] = b;
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
        colorinc();
    }

    cinterval = speed+1;
    float brightnessFactor = (float)(((float)brightness)/100);
    int r = colors[0] * brightnessFactor;
    int g = colors[1] * brightnessFactor;
    int b = colors[2] * brightnessFactor;

    setWS2801Strip(r, g, b);
}

void updateWS2801()
{
    for(int i = 0; i < striplen; i++)
    {
        SPI.transfer(buffer[0][i]);
        SPI.transfer(buffer[1][i]);
        SPI.transfer(buffer[2][i]);
    }
    delayMicroseconds(500);
}