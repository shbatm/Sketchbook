//esp-led-nondigital-testing.ino
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

// inlcudes ssid and passwords.
#include "initials.h"
// includes the ota function to use for uploading
// over the wifi.
#include <otaupload.h>
// includes a few setup functions to make it easier.
// to setup a connection.
#include <esputils.h>

Ticker ledtick;

volatile int value = 0;
volatile int value2 = 0;
volatile int value3 = 0;

#define GREEN 4
#define BLUE 5
#define RED 2

int prevled = 0;
int ledselect = 1;
int leds[] = {RED, GREEN, BLUE};

int getRandomVal()
{
  int value = random(0, 0x400);
  while(value > 0x400)
  {
    value = random(0, 0x400);
  }
  return value;
}

void pwmhandle()
{
  value = getRandomVal();
  value2 = getRandomVal();
  value3 = getRandomVal();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  // configure pins.
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(0, INPUT_PULLDOWN);
  // start up in station mode.
  setupSTA();
  // enabled ota uploading.
  setupOta();
  // set hanlde for led ticker.
  ledtick.attach(1, pwmhandle);
}

void loop() {
  handleSketchUpdate();
  while(!digitalRead(0))
  {
    handleSketchUpdate();
    delay(0);
    Serial.println("Hello World!");
  }
  analogWrite(RED, value);
  analogWrite(GREEN, value2);
  analogWrite(BLUE, value3);
}