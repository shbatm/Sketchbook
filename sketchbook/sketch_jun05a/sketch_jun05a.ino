#include <lightpaper.h>

void setup()
{
    setupLightPaper();
    putStr("HelloHelloWorld!");
    Serial.begin(115200);
    testBuffer();
}

void testBuffer()
{
    // test works jeej!
    uint8_t *bp_p = getLPBuffer();
    for(int i = 0;i < getLPBufferSize(); i++)
    {
        (*bp_p) = 0xFF;
        bp_p++;
    }
}

void loop()
{
    // if(Serial.available() >= 0)
    // {
    //     uint8_t *bp_p = getLPBuffer();
    //     (*bp_p) = Serial.read();
    //     bp_p++;
    // }
    writeout();
}