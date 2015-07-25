#ifndef HTML_H
#define HTML_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include "stripcontrol.h"

// declared in main sketch.
extern String sta_ssid;
extern String sta_pass;
extern String ap_ssid;
extern String ap_pass;

extern ESP8266WebServer server;

extern stripcontrol_t stripcontrol;

void handleStripControl();
void handleWiFiSettings();
void handleLedSettings();
void handleRoot();

#endif
