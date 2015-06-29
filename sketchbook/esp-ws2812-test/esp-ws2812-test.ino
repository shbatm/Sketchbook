#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "initials.h"
#include <otaupload.h>
#include <esputils.h>

#include <SPI.h>
#define DATA MOSI

#define WS2812ZERO 0x80
#define WS2812ONE  0xF9
#define WS2812_PAUSE_SIZE 64
uint8_t pause_frame[WS2812_PAUSE_SIZE];
uint8_t data = WS2812ONE;
const uint16_t buffersize = 64;
uint8_t writebuffer[buffersize] = {data, data};

class WS2812
{
  public:
    WS2812();
};

WS2812::WS2812()
{

}

void setupWS2812()
{
  pause_frame[0] = WS2812ZERO;
  pause_frame[WS2812_PAUSE_SIZE] = 0x01;

  SPI.begin();
  SPI.setFrequency(8e6*0.8);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  setupSTA();
  setupOta();
  pinMode(0, INPUT_PULLDOWN);

  setupWS2812();
  for(int i = 0; i< buffersize;i++)
  {
    uint8_t rand = random(0, 2);
    if(rand)
    {
      writebuffer[i] = WS2812ONE;
    }
    else
    {
      writebuffer[i] = WS2812ZERO;
    }
  }
  SPI.writePattern(writebuffer, buffersize, 1);
  SPI.writePattern(pause_frame, WS2812_PAUSE_SIZE, 1);
}

void loop()
{
  handleSketchUpdate();
  while(!digitalRead(0))
  {
    handleSketchUpdate();
    Serial.println("Hello World!");
    delay(0);
  }
}
