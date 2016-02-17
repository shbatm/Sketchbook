#include <GDBStub.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <SD.h>
#include <SdFat.h>
#include <SdFatUtil.h>

#include "settings.h"
#include "ota.h"
#include "cmd.h"
#include "webserver.h"
#include "settingsStore.h"


String board_name = "3Dprinter";
String ssid = "ssid";
String pass = "pass";

WiFiMode currentMode = WIFI_STA;

const int chipSelect = 4;
bool hasSD = false;

String modeToStr(WiFiMode wifimode)
{
    switch(wifimode)
    {
        case WIFI_STA:
            return String("WIFI_STA");
        case WIFI_AP:
            return String("WIFI_AP");
        case WIFI_AP_STA:
            return String("WIFI_AP_STA");
        default:
            return String("no valid mode set");
    }
}

void setupWifi()
{
    WiFi.mode(currentMode);
    WiFi.disconnect();
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("connecting");
    int breakout = 0;
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.write('.');
        delay(500);
        breakout += 1;
        if(breakout == 20)
        {
            Serial.println("");
            Serial.printf("Couldn't connect to: %s\n", ssid.c_str());
            return;
        }
    }
    Serial.println("");
    WiFi.hostname(board_name.c_str());
    Serial.println("connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void setupSD()
{
    Serial.println("setting up SD.");
    // check for a card.
    // open at X MHz.
    if(!SD.begin(chipSelect, 25e6))
    {
        Serial.println("sd card failed to begin library.");
        hasSD = false;
    }
    else
    {
        Serial.println("sd begin succeeded.");
        hasSD = true;
    }
}

void setupNetworkServices()
{
    Serial.println();
    Serial.println("setting up wifi.");
    setupWifi();
    Serial.println("setup ota.");
    setupOTA();
    Serial.println("setup http server.");
    setupWebServer();
}

void storeSettings()
{
    strcpy(settings.board_name, board_name.c_str());
    strcpy(settings.ssid, ssid.c_str());
    strcpy(settings.pass, pass.c_str());
    storeStruct(&settings, sizeof(settings));
}

void loadSettings()
{
    loadStruct(&settings, sizeof(settings));
    board_name = settings.board_name;
    ssid = settings.ssid;
    pass = settings.pass;
}

void setup()
{
    Serial.begin(576000);
    Serial.println("\n");

    loadSettings();
    setupSD();
    setupNetworkServices();
    setupInterpreter();
}

void loop()
{

    // get new input and interpret it.
    interpretInput(getInput());
    // handle ota for firmware upload
    ota.handle();
    // handle server.
    handleServerClient();
    yield();
}
