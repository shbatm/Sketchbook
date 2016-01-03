#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

#include "cmd.h"
#include "webserver.h"
#include "settingsStore.h"

String board_name = "3Dprinter";
String ssid = "ssid";
String pass = "pass";


int currentMode = WIFI_STA;

const int chipSelect = 4;

bool hasSD = false;

ArduinoOTA ota_server;

String modeToStr(int wifimode)
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

void settingsStore()
{
    Serial.println();
    Serial.println("storing: ");
    int addr = 0;
    storeMagicStr(addr);
    Serial.printf("board_name: %s\n", board_name.c_str());
    storeString(board_name, addr);
    Serial.printf("ssid: %s\n", ssid.c_str());
    storeString(ssid, addr);
    Serial.printf("pass: %s\n", pass.c_str());
    storeString(pass, addr);
    Serial.printf("wifimode: %s\n", modeToStr(currentMode).c_str());
    storeInt(currentMode, addr);
    storeDone();
    Serial.println();
}

void settingsLoad()
{
    Serial.println();
    Serial.println("loading: ");
    int addr = 0;
    if(magicStrPresent(addr))
    {
        Serial.println("valid settings!");
        board_name = loadString(addr);
        Serial.printf("board_name: %s\n", board_name.c_str());
        ssid = loadString(addr);
        Serial.printf("ssid: %s\n", ssid.c_str());
        pass = loadString(addr);
        Serial.printf("pass: %s\n", pass.c_str());
        currentMode = loadInt(addr);
        Serial.printf("wifimode: %s\n", modeToStr(currentMode).c_str());
        Serial.println();
    }
    else
    {
        Serial.println("no valid settings!");
        settingsStore();
        Serial.println();
    }
}

void setupWifi()
{
    WiFi.mode(WIFI_STA);
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

void setupOTA()
{
    ota_server.setup();
}

void setupSD()
{
    // check for a card.
    if(!SD.begin(chipSelect))
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
    Serial.println("setting up wifi.");
    setupWifi();
    Serial.println("setup ota.");
    setupOTA();
    Serial.println("setup http server.");
    setupWebServer();
}

void setup()
{
    Serial.begin(115200);
    Serial.println("");
    EEPROM.begin(1024);
    settingsLoad();

    Serial.println("setting up SD.");
    setupSD();

    setupNetworkServices();
}

void loop()
{

    // get new input and interpret it.
    interpretInput(getInput());
    // handle ota for firmware upload
    ota_server.handle();
    // handle server.
    handleServerClient();
    yield();
}