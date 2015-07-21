#include <lightpaper.h>

uint8_t buff[80];
uint8_t bi = 0;
uint8_t *bp;

bool ledstate = false;
unsigned long current = 0;
unsigned long previous = 0;
int interval = 1000;

void setup()
{
    setupLightPaper();
    Serial.begin(250000);
    putStr("Gello");
    pinMode(13, OUTPUT);
    bp = getLPBuffer();
}

void loop()
{
    Serial.write('n');
    while(Serial.available() == 0);
    if(Serial.available() && Serial.read() == 'f')
    {
        uint8_t index = 0;
        while(Serial.available())
        {
            bp[index++] = Serial.read();
        }
    }
    writeout();
}