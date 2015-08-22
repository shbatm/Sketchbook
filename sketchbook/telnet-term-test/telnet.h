#ifndef TELNET_H
#define TELNET_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define TELNETPORT 1337

static WiFiClient telnet;
static WiFiServer telnetServer(TELNETPORT);

void setupTelnet();
void handleTelnet();
void sendTelnet(String);

#endif