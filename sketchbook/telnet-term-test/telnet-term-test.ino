#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include "telnet.h"

const char *ssid = "www.tkkrlab.nl";
const char *pass = "hax4or2the2paxor3";

#define SINGLE 1
#define DIFFERENTIAL 0
#define SIGLDIF(X) (X << 9)
#define SELPIN 4

uint16_t readAnalogChannel(int channel)
{
    int adcvalue = 0;
  int b1 = 0, b2 = 0;
  int sign = 0;

  // command bits for MCP3304
  // 0000 = diff, ch0 = in+, ch1 = in-
  // 0010 = diff, ch2 = in+, ch3 = in-
  // 0100 = diff, ch4 = in+, ch5 = in-

  digitalWrite (SELPIN, LOW); // Select adc
  delay(0);

  // first byte
  // first byte will always be B000010xx where xx are the D2 and D1 channel bits  
  byte commandbits = B0000110;
  commandbits |= (channel >> 2);         // high bit of channel

  SPI.transfer(commandbits);       // send out first byte of command bits

  // second byte; Bx0000000; leftmost bit is D0 channel bit
  commandbits = B00000000;
  commandbits |= (channel << 6);        // if D0 is set it will be the leftmost bit now
  b1 = SPI.transfer(commandbits);       // send out second byte of command bits

  // hi byte will have XX0SBBBB
  // set the top 3 don't care bits so it will format nicely
  b1 |= B11100000;
  //Serial.print(b1, BIN); Serial.print(" ");
  sign = b1 & B00010000;
  int hi = b1 & B00001111;

  // read low byte
  b2 = SPI.transfer(b2);              // don't care what we send
  //Serial.print(b2, BIN); Serial.print("\r\n");
  int lo = b2;
  digitalWrite(SELPIN, HIGH); // turn off device
  delay(0);

  int reading = hi * 256 + lo;

  if (sign) {
    reading = (4096 - reading) * -1;
  }

  return (reading);
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
        int i = 0;
        for(i; i<=7; i++)
            sendTelnet(String("channel(") + String(i) + ") " + String(readAnalogChannel(i)) + "\n");
    }
    handleTelnet();
}