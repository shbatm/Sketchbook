#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>

const char *ssid = "www.tkkrlab.nl";
const char *pass = "hax4or2the2paxor3";

ADC_MODE(ADC_VCC);

#define SINGLE 1
#define DIFFERENTIAL 0
#define SIGLDIF(X) (X << 1)
#define STARTBIT (1 << 2)
#define SELPIN 5

WiFiUDP GCU;

// typedef struct
// {
//   int value;
//   int offset;
//   int range;
// } adcChanSpec_t;

// adcChanSpec_t rotation {0, 0, 0};

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

int readRangedAdcChannel(int ch, int range, bool isInv)
{
  int adcVal = readAnalogChannel(ch);
  int ranged = map(adcVal, 0, 4096, 0, range);
  if(isInv)
  {
    return (180 - ranged);
  }
  else
  {
    return ranged;
  }
}

void sendAdcPacket()
{
  static int previous;
  static int current;
  static int offset = 2;

  int rotation = readAnalogChannel(1);
  int rotStart = 0;
  int rotEnd = 4096;
  rotation = map(rotation, rotStart, rotEnd, 0, 180);

  int bottom = readAnalogChannel(2);
  int botStart = 1600;
  int botEnd = 2800;
  if(bottom < botStart)
  {
    bottom = botStart;
  }
  bottom = map(bottom, botStart, botEnd, 0, 180);
  bottom = 180 - bottom;

  int head = readAnalogChannel(0);
  int headStart = 1470;
  int headEnd = 3100;
  head = map(head, headStart, headEnd, 0, 180);
  if(head < 50)
  {
    head = 50;
  }
  if(head > 130)
  {
    head = 130;
  }
  // invert motion on the head.
  head = 180 - head;

  int base = readAnalogChannel(3);
  int baseStart = 1600;
  int baseEnd = 4096;
  if(base < baseStart)
  {
    base = baseStart;
  }
  base = map(base, baseStart, baseEnd, 0, 180);

  // Serial.printf("0:%d, 1:%d, 2:%d, 3:%d \n",
  //                 rotation,
  //                 bottom,
  //                 head,
  //                 base);

  current = (rotation + bottom + head + base);
  bool isSame = ((previous < (current+offset)) && (previous > (current - offset)));
  if(!isSame)
  {
    // Serial.printf("isSame: %s \n", isSame ? "True" : "False");

    // Serial.printf("0:%d, 1:%d, 2:%d, 3:%d \n",
    //               rotation,
    //               bottom,
    //               head,
    //               base);

    GCU.beginPacket("10.42.4.185", 1337);
    GCU.write(rotation);
    GCU.write(bottom);
    GCU.write(head);
    GCU.write(base);
    GCU.endPacket();
  }

  previous = current;
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    pinMode(SELPIN, OUTPUT);
    digitalWrite(SELPIN, HIGH);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Ip: ");
    Serial.println(WiFi.localIP());

    GCU.begin(1337);

    SPI.setFrequency(10e6);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.begin();
}

void loop()
{
  sendAdcPacket();
  delay(100);
}