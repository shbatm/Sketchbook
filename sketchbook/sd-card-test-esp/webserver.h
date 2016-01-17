#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h>

#include "serverprintaction.h"

void setupWebServer();
void returnOK();
String loadFile(String path);
void uploadHtml();
void uploadGcode();
void upload();
void notFound();
void handleFileUpload();
void handleServerRoot();
void handleSDFileMan();
void handleServerClient();


#endif