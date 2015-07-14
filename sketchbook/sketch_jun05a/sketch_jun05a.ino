#include <lightpaper.h>

uint8_t buff[80];

bool ledstate = false;

void setup()
{
    setupLightPaper();
    putStr("Gello");
    Serial.begin(115200);
    pinMode(13, OUTPUT);
}

void loop()
{
    Serial.print("f");
    // if(Serial.available() > 0)
    // {
    //     char readb = Serial.read();
    //     for(int i = 0; i < 80; i++)
    //     {
    //         Serial.write(i);
    //     }
    // }
}