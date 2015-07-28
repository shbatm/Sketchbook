//ldp8806-esp.ino

#include <SPI.h>

int len = 30;
uint8_t *buffer[3] = {NULL, NULL, NULL};

void updateLength()
{
    for(int i = 0; i < len*3; i++)
    {
        uint8_t r = buffer[0][i] | 0x80;
        uint8_t g = buffer[1][i] | 0x80;
        uint8_t b = buffer[2][i] | 0x80;
        resetStrip();
    }
}

void resetStrip()
{
    for(int i = ((len+31)/32); i > 0; i--)
    {
        SPI.transfer(0x00);
    }
}

void setup()
{
    SPI.begin();
    SPI.setFrequency(1e6);
    SPI.setDataMode(SPI_MODE0);
    buffer[0] = (uint8_t *)malloc(sizeof(uint8_t)*len);
    buffer[1] = (uint8_t *)malloc(sizeof(uint8_t)*len);
    buffer[2] = (uint8_t *)malloc(sizeof(uint8_t)*len);
    for(int i = 0; i < len; i++)
    {
        buffer[2][i] = 0xff;
    }
}

void loop()
{
    updateLength();
}