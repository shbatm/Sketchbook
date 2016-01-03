#include "serverprintaction.h"

extern ESP8266WebServer server;

void returnOK()
{
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "");
}

void action()
{
    if(server.args())
    {
        for(int i = 0; i < server.args(); i++)
        {
            Serial.printf("arg: %s=%s \n",
                          server.argName(i).c_str(),
                          server.arg(i).c_str());
        }
    }
    returnOK();
}