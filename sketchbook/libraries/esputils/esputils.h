#ifndef ESPUTILS_H
#define ESPUTILS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

extern const char* ssid;
extern const char* pass;
extern const char* ap_ssid;
extern const char* ap_pass;

void setupSTA();
void setupAP();
void checkConnected();
void printWifiStatus();

#endif