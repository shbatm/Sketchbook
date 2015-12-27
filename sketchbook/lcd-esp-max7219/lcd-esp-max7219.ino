//We always have to include the library
#include <SPI.h>
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
//LedControl lc=LedControl(12,11,10,1);

LedControl lc = LedControl(13, 14, 4, 1);

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
}

unsigned long current = 0;
unsigned long previous = 0;
int interval = 100;
uint8_t intensity;

void setLed(uint8_t x, uint8_t y)
{
  lc.setLed(0, x, y, true);
}

void clearLed(uint8_t x, uint8_t y)
{
  lc.setLed(0, x, y, false);
}

void loop() {
  setLed(0, 0);
  // for(int i = 0 ; i< 64; i++)
  // {
  //   setLed(random(0, 8), random(0, 8));
  //   clearLed(random(0, 8), random(0, 8));
  // }
  // delay(100);
//  lc.setLed(0, 0, 0, true);
//  lc.setLed(0, 1, 1, true);
//  lc.setLed(0, 2, 2, true);
//  
//  current = millis();
//  if(current - previous >= interval)
//  {
//    previous = current;
//    lc.setIntensity(0, intensity);
//    intensity++;
//    if(intensity > 16)
//    {
//      intensity = 0;
//    }
//  }
}
