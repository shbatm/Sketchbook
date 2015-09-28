#include <lightpaper.h>

static char input[20];

void setup()
{
    setupLightPaper();
    Serial.begin(250000);
}

bool dataStarted = false;
String inputString = "";

void loop()
{
    // static char input[100];
    // if(Serial.available())
    // {
        
    //     uint8_t strindex = 0;
    //     while(Serial.available() != 0)
    //     {
    //         char inChar = Serial.read();
    //         Serial.write(inChar);
    //         if(isprint(inChar))
    //         {
    //             input[strindex] = inChar;
    //             input[strindex+1] = '\0';
    //             putChar(inChar, strindex*5);
    //             strindex++;
    //         }
    //         else if(strindex > 14)
    //         {
    //             // clear serial buffer and break;
    //             while(Serial.available())
    //             {
    //                 Serial.read();
    //             }
    //             break;
    //         }
    //         else
    //         {
    //             // clear serial buffer and break;
    //             while(Serial.available())
    //             {
    //                 Serial.read();
    //             }
    //             break;
    //         }
    //     }
    // }

    // check if any incomming data.
    // if(Serial.available())
    // {
    //     // wait for the start of a string.
    //     if(Serial.read() == 0x02);
    //     {
    //         // read in string till we read a end of string.
    //         uint8_t ti = 0;
    //         char read = Serial.read();
    //         while(read != 0x03)
    //         {
    //             if(isprint(read))
    //             {
    //                 input[ti++] = read;
    //                 input[ti] = '\0';
    //             }
    //             read = Serial.read();
    //         }
    //     }
    // }

    if(Serial.available())
    {
        if (dataStarted) {
            while(Serial.available()) {
                char serin = Serial.read();
                if (serin==0x03) {
                    dataStarted = false;
                    putStr((char*) inputString.c_str());
                } else {
                    inputString = inputString + String(serin);
                }
            }
        } else {
            while(Serial.available()) {
                char serin = Serial.read();
                if (serin==0x02) {
                    dataStarted = true;
                    inputString = "";
                    break;
                }
            }
        }
    }
    writeout();
}