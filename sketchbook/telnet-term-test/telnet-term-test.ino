#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include "telnet.h"

const char *ssid = "www.tkkrlab.nl";
const char *pass = "hax4or2the2paxor3";

uint8_t adcFrame[3];

#define SINGLE 1
#define DIFFERENTIAL 0
#define STARTBIT (1 << 2)
#define SNGLDIFBIT(X) (X <<  1)

#define SELPIN 4

uint16_t readAnalogChannel(int channel)
{
    uint8_t command;
    uint16_t value;
    // clear adcFrame
    memcpy(adcFrame, "0", 3);
    // make sure we only have 8 channels.
    channel &= 0x07;

    // trigger a conversion.
    digitalWrite (SELPIN, LOW);
    delay(0);
    
    // setup first command byte.
    command = STARTBIT | SNGLDIFBIT(SINGLE) | (channel >> 2);
    adcFrame[0] = command;

    // setup second command byte.
    command = (channel << 7);
    adcFrame[1] = command;

    // setup third command byte, doesn't matter what it is.
    command = 0x00;
    adcFrame[2] = command;

    // transfer bytes and put the results in the adcFrame.
    SPI.transferBytes(adcFrame, adcFrame, 3);

    digitalWrite(SELPIN, HIGH);
    delay(0);

    value = (adcFrame[1] | (adcFrame[2] << 8)) & 0xFFF;

    return value;
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    setupTelnet();

    Serial.print("Ip: ");
    Serial.println(WiFi.localIP());

    pinMode(SELPIN, OUTPUT);
    digitalWrite(SELPIN, HIGH);
    SPI.setFrequency(10e6);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.begin();
}

static unsigned long current, previous;
static int interval = 1000;

void telnetsendtest()
{
    current = millis();
    if(current - previous >= interval)
    {
        previous = current;
        sendTelnet("Test\n");
    }
}

void loop()
{
    current = millis();
    if(current - previous >= interval)
    {
        previous = current;
        Serial.println();
        for(int i = 0; i<=7; i++)
        {
             sendTelnet(String("channel(") + String(i) + ") " + String(readAnalogChannel(i)) + "\n");
        }
    }
    handleTelnet();
}