#include <lightpaper.h>

void setup()
{
    setupLightPaper();
    Serial.begin(250000);
}

bool foundData = false;
String inputString = "";

uint8_t input[80];
int index = 0;

void loop()
{
    // if(Serial.available())
    // {
    //     if (dataStarted) {
    //         while(Serial.available()) {
    //             char serin = Serial.read();
    //             if (serin==0x83) {
    //                 dataStarted = false;
    //                 //putStr((char*) inputString.c_str());
    //                 writeLPBuffer(input, 80);
    //             } else {
    //                 //inputString = inputString + String(serin);
    //                 input[index] = serin;
    //                 index++;
    //             }
    //         }
    //     } else {
    //         while(Serial.available()) {
    //             char serin = Serial.read();
    //             if (serin==0x82) {
    //                 dataStarted = true;
    //                 //inputString = "";
    //                 index = 0;
    //                 break;
    //             }
    //         }
    //     }
    // }
    if(Serial.available())
    {
        uint8_t data = Serial.read();
        if(data == 0x82)
        {
            foundData = true;
        }
        else if(data == 0x83)
        {
            putStr("it")
            index = 0;
            foundData = false;
            // writeLPBuffer(input, 80);
        }
        // else if(foundData)
        // {
        //     input[index] = data;
        //     index++;
        // }
    }
    writeout();
}