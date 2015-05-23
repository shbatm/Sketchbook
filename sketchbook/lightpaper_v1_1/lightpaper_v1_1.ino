//lightpaper_v1_1.ino
#include <EtherCard.h>
#include "lightpaper.h"

#define ENCRESET 9
#define ENCPIN 8

byte mac[] = {'l','i','c','h','t','0'};
char hostname[] = "lightpaper";

byte Ethernet::buffer[400];
static BufferFiller bfill;

char *data;

const char page[] PROGMEM = 
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 1\r\n"
"\r\n"
"<html>"
    "<head><title>"
        "Lightpaper Input."
    "</title></head>"
    "<body>"
        "input text:<br>"
        "<form name=\"\" method=\"get\">"
            "<input type=\"text\" name=\"textinputfield\"><br>"
            "<input type=\"submit\", value=\"Submit\">"
        "</form>"
    "</body>"
"</html>"
;

const char http_OK[] PROGMEM =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"Pragma: no-cache\r\n\r\n";

const char http_Found[] PROGMEM =
"HTTP/1.0 302 Found\r\n"
"Location: /\r\n\r\n";

const char http_Unauthorized[] PROGMEM =
"HTTP/1.0 401 Unauthorized\r\n"
"Content-Type: text/html\r\n\r\n"
"<h1>401 Unauthorized</h1>";

void setupEnc()
{
    pinMode(ENCRESET, OUTPUT);
}

void resetEnc()
{
    digitalWrite(ENCRESET, LOW);
    delay(100);
    digitalWrite(ENCRESET, HIGH);
    delay(100);
}

void setup()
{
    setupEnc();
    resetEnc();
    ether.begin(sizeof Ethernet::buffer, mac);
    ether.dhcpSetup(hostname, true);

    setupLightPaper();
    putStr(hostname);
}

void parseUrlDecode(char *input, char *buffer)
{
    //extract data/text/string
    for(int i = 0;input[i]!=' ';i++)
    {
        buffer[i] = input[i];
    }
    //decode the string in the buffer
    ether.urlDecode(buffer);
}

unsigned long current = millis();
unsigned long previous = 0;
uint8_t interval = 5;

void loop()
{
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    if(pos)
    {
        delay(1);
        bfill = ether.tcpOffset();
        data = (char *)Ethernet::buffer + pos;
        if(strncmp("GET /", data, 5) != 0)
        {
            memcpy_P(ether.tcpOffset(), http_Unauthorized, sizeof http_Unauthorized);
            ether.httpServerReply(sizeof http_Unauthorized - 1);
        }
        else
        {
            data += 5;
            if(data[0] == ' ')
            {
                memcpy_P(ether.tcpOffset(), page, sizeof page);
                ether.httpServerReply(sizeof page - 1);
            }
            else if (strncmp("?textinputfield=", data, 5) == 0)
            {
                data+=strlen("?textinputfield=");
                clearLPStrBuffer();
                clearLPBuffer();
                parseUrlDecode(data, getLPStrBuffer());
                putStr(getLPStrBuffer());
                memcpy_P(ether.tcpOffset(), http_Found, sizeof http_Found);
                ether.httpServerReply(sizeof http_Found - 1);
            }
            else
            {
                memcpy_P(ether.tcpOffset(), http_Unauthorized, sizeof http_Unauthorized);
                ether.httpServerReply(sizeof http_Unauthorized - 1);
            }
        }
    }
    current = millis();
    if(current - previous > interval)
    {
        previous = current;
        if(strlen(getLPStrBuffer()) > getLPMaxLetterWidth())
        {
            marquee(getLPStrBuffer());
        }
        else
        {
            writeout();
        }
    }
}