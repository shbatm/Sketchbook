#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

#include "initials.h"
#include <otaupload.h>
#include <esputils.h>

#include <SPI.h>

#define NUMPIXELS 16
uint8_t buffer[3][NUMPIXELS];

int striplen = NUMPIXELS;

void setupWS2801(int freq, int len)
{
    SPI.begin();
    SPI.setFrequency(1e6);
}

void setWS2801Pixel(int pos, int r, int g, int b)
{
    buffer[0][pos] = r;
    buffer[1][pos] = g;
    buffer[2][pos] = b;
}

void setWS2801Stip(int r, int g, int b)
{

}

void updateWS2801()
{
    for(int i = 0; i < striplen; i++)
    {
        SPI.transfer(buffer[0][i]);
        SPI.transfer(buffer[1][i]);
        SPI.transfer(buffer[2][i]);
    }
}

void setup()
{
    Serial.begin(115200);

    setupWS2801(1e6, NUMPIXELS);

    setupSTA();
    setupOta();

    setWS2801Pixel(0, 42, 13, 168);
    setWS2801Pixel(NUMPIXELS-1, 0, 0, 255);

    Serial.println("\nEntering Loop");
}

void loop()
{
    handleSketchUpdate();
    updateWS2801();
}