#include <ESP8266WiFi.h>

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if(Serial.available() > 0)
    {
        Serial.print("h");
        Serial.write(Serial.read());
        // char readb = Serial.read();
        // if(readb == 'f')
        // {
        //     for(int i = 0; i < 80; i++)
        //     {
        //         Serial.write(i);
        //     }
        // }
        // else
        // {
        //     while(Serial.available() > 0) Serial.read();
        // }
    }
    Serial.print("t");
}
