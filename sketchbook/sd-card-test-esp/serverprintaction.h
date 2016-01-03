#ifndef __SERVERPRINTACTION_H__
#define __SERVERPRINTACTION_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

extern String loadHtml(String);
void returnOK();
void action();

#endif
