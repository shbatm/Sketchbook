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