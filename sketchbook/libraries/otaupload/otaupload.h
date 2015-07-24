#ifndef OTAUPLOAD_H
#define OTAUPLOAD_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

static WiFiUDP listener;

void setupOta();
int handleSketchUpdate();

#endif