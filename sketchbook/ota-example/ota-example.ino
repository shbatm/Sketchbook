#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "initials.h"
#include <otaupload.h>
#include <esputils.h>


void setup()
{
    Serial.begin(115200);
    Serial.println();
    setupSTA();
    setupOta();
}

void loop()
{
    handleSketchUpdate();
}