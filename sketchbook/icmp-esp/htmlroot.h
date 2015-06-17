#ifndef HTMLROOT_H
#define HTMLROOT_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

void handleWiFiSettings();
void handleLedSettings();
void handleRoot();

#endif
