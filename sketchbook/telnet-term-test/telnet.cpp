#include "telnet.h"

void setupTelnet()
{
    // start the telnet server.
    telnetServer.begin();
    telnetServer.setNoDelay(true);
}

void sendTelnet(String text)
{
    if(text)
    {
        size_t length = text.length();
        if(telnet && telnet.connected())
        {
            telnet.print(text);
            delay(1);
        }
    }
}

void handleTelnet()
{
    //IDE Monitor (connected to Serial)
    if (telnetServer.hasClient())
    {
        if(!telnet || !telnet.connected())
        {
            if(telnet) telnet.stop();
            telnet = telnetServer.available();
        }
        else
        {
            WiFiClient toKill = telnetServer.available();
            toKill.stop();
        }
    }
    // if(telnet && telnet.connected() && telnet.available())
    // {
    //     while(telnet.available())
    //     {
    //         Serial.write(telnet.read());
    //         delay(1);
    //     }
    // }
}