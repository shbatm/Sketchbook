#include <lightpaper.h>

void setup()
{
    setupLightPaper();
    Serial.begin(250000);
}

void loop()
{
    static int index;
    if(Serial.available())
    {
        char inbyte = Serial.read();
        if(isprint(inbyte))
        {
            putChar(inbyte);
            index+=5;
        }
        if(index > (16*5))
        {
            index = 0;
        }
    }
    writeout();
}