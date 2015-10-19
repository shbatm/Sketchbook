/*
  Author: Duality / Robert

  This is Firmware for controlling ledstrips.
  It includes a way of setting which strip is connected,
  and how long that ledstrip is.
  It also includes a Way of finding the device through
  broadcastig a udp packet.
  It also includes a way for controlling which effect is selected.
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include <otaupload.h>

#include "stripcontrol.h"
#include "html.h"
#include "effectParse.h"

#define AP_BUTTON       0// 12
#define SERIALBAUD      115200
#define EFFECTPORT      1337
#define WEBSERVERPORT   80
#define EEPROMSIZE      1024
#define SERVERTEST      true

// set initial board name and wifi settings.
String board_name = "TkkrEsp-01";
String sta_ssid = "www.tkkrlab.nl";
String sta_pass = "hax4or2the2paxor3";

// select an initial mode.
enum {STA_MODE, AP_MODE};
int currentMode = STA_MODE;

// set an initial pincode.
int accessPin = 1234;

// create a stripcontrol structure and clear it.
stripcontrol_t stripcontrol = {
  .pincode = 0,
  .effect = 0,
  .brightness = 0,
  .varZero = 0,
  .varOne = 0,
  .varTwo = 0,
  .changed = false
};


// select intial ledstrip
int stripselect = ANALOGSTRIP;
// select an initial length.
int striplen = 1;

/*
  stored are:
  board_name,
  sta ssid,
  sta pass,
  accesPin,
  stripselect,
  currentMode
  */
typedef struct
{
  String board_name;
  String sta_ssid;
  String sta_pass;
  uint16_t accesPin;
  uint8_t stripselect;
  uint8_t currentMode;
} board_settings_t;

board_settings_t default_settings
{
  .board_name = String("EspLight"),
  .sta_ssid = String("EspLight"),
  .sta_pass = String("EspLight"),
  .accesPin = 0,
  .stripselect = ANALOGSTRIP,
  .currentMode = AP_MODE
};

ESP8266WebServer server(WEBSERVERPORT);

// stores a string into eeprom plus nullterminator.
void storeString(String string, int& addr)
{
  // get a reference to the original c string.
  const char *str = string.c_str();
  // get the length of that string.
  int str_len = strlen(str);
  int i;
  // loop over the length of the string.
  // and store the bytes. and zero terminator.
  for(i = 0; i <= str_len; i++)
  {
    // write the bytes into the eeprom (flash)
    EEPROM.write(addr+i, str[i]);
  }
  addr += i;
}

void storeInt(int value, int& addr)
{
  char fmtstr[100];
  int i;
  for(i = 0; i < sizeof(value); i++)
  {
    EEPROM.write(addr+i, (value>>(i*8)&0xff));
  }
  addr += i;
}

String loadString(int &addr)
{
  String text = "";
  char read = EEPROM.read(addr);
  /* Basicly how you read any string in C. */
  while(read != '\0')
  {
    text += read;
    addr++;
    read = EEPROM.read(addr);
  }
  addr++; //acount for zero terminator.
  return text;
}

int loadInt(int &addr)
{
  int value = 0;
  int i;
  for(i = 0; i < 0x04; i++)
  {
    char byte = EEPROM.read(addr+i);
    value |= (byte << (8*i));
  }
  addr += i;
  return value;
}

void settingsStore()
{
  /*
  stored are:
  board_name,
  sta ssid,
  sta pass,
  accesPin,
  stripselect,
  currentMode
  */
  int eeAddr = 0;
  storeString(board_name, eeAddr);
  storeString(sta_ssid, eeAddr);
  storeString(sta_pass, eeAddr);
  storeInt(accessPin, eeAddr);
  storeInt(stripselect, eeAddr);
  storeInt(striplen, eeAddr);
  EEPROM.commit();
}

void settingsLoad()
{
  int eeAddr = 0;
  board_name = loadString(eeAddr);
  sta_ssid = loadString(eeAddr);
  sta_pass = loadString(eeAddr);
  accessPin = loadInt(eeAddr);
  stripselect = loadInt(eeAddr);
  striplen = loadInt(eeAddr);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setupAP()
{
  currentMode = AP_MODE;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(board_name.c_str());
  Serial.println();
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

void setupSTA(bool silent)
{
  currentMode = STA_MODE;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());
  if(!silent)
  Serial.println();
  // timeout variable.
  int i = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if(!silent)
    Serial.print(".");
    // if button pressed switch mode.
    wifiModeHandling();
    // keep a timeout timer.
    i++;
    if(i == 10)
    {
      if(!silent)
      Serial.println("Unable to connect");
      return;
    }
  }
  if(!silent)
  Serial.println();
  printWifiStatus();
}

void wifiModeHandling()
{
  if(!digitalRead(AP_BUTTON))
  {
    // switch modes as needed.
    if(currentMode == STA_MODE)
    {
      Serial.println("mode is now AP_MODE");
      setupAP();
    }
    /* actually never gets here. but does if we allowed it to switch back.*/
    // else
    // {
    //   Serial.println("mode is now STA_MODE");
    //   // list to output that we are connecting
    //   setupSTA(false);
    // }
    while(!digitalRead(AP_BUTTON)) delay(50);
  }
}

void setupWifi(bool silent)
{
  if(currentMode == STA_MODE)
  {
    Serial.println("mode is now STA_MODE");
    setupSTA(silent);
  }
  else
  {
    Serial.println("mode is now AP_MODE");
    setupAP();
  }
}

void setupWebserver()
{
  // attach directories and file handlers.
  server.on("/", handleRoot);
  server.on("/ledsettings", handleLedSettings);
  server.on("/wifisettings", handleWiFiSettings);
  server.on("/css", handleCss);
  // start the server
  server.begin();
  Serial.println("done setting up server");
  // request a hostname
  WiFi.hostname(board_name);
  Serial.printf("Hostname set: %s\n", board_name.c_str());
}

void setup() {
  Serial.begin(SERIALBAUD);
  Serial.println();
  // prepare eeprom for use.
  EEPROM.begin(EEPROMSIZE);
  // settingsStore();
  // load stored settings.
  settingsLoad();

  // setup mode switching pin
  pinMode(AP_BUTTON, INPUT_PULLUP);

  // setup strips for the first time (initialize some pointers and stuff.)
  setupStrips(striplen);

  // setup wifi with output.
  setupWifi(false);
  // enable OTA
  Serial.setDebugOutput(true);
  setupOta();

  Serial.println("done setting up pins, and WifiMode.");

  /* Setup server side things.*/
  setupWebserver();
  // setup the effect parser.
  setupEffectParse(EFFECTPORT);
}

void loop() {
  if((currentMode == AP_MODE) || SERVERTEST)
  {
    // only serve pages in Access point mode.
    server.handleClient();
  }
  if(currentMode == STA_MODE)
  {
    // check if we are still connected.
    if(WiFi.status() != WL_CONNECTED)
    {
      // don't list anything to the serial output.
      // but still try to connect.
      setupSTA(true);
      if(WiFi.status() == WL_CONNECTED)
      {
        // if reconnected restart anything that uses a port.
        setupOta();
        setupWebserver();
        setupEffectParse(EFFECTPORT);
      }
    }
    // enable ledstrip animations.
    handleStrips();
    // enable controle over effects.
    handleEffectUpdate();
    // enable switching to AP_MODE
    wifiModeHandling();
  }
  // allow uploading over OTA <dev>
  handleSketchUpdate();
}
