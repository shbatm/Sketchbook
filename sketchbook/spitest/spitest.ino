//spitest.ino

#include <SPI.h>

void setup()
{
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
}

void loop()
{
    SPI.transfer(42);
}