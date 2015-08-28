#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "html.h"
#include "stripcontrol.h"

#define AP_BUTTON       12
#define SERIALBAUD      115200
#define EFFECTPORT      1337
#define WEBSERVERPORT   80
#define EEPROMSIZE      1024
#define OTAPORT         48266
#define SERVERTEST      true


// set initial board name and wifi settings.
String board_name = "TkkrEsp-01";
String sta_ssid = "www.tkkrlab.nl";
String sta_pass = "apassword";

// set an initial pincode.
int accessPin = 1234;

stripcontrol_t stripcontrol = {
  .pincode = 0,
  .effect = 0,
  .brightness = 0,
  .varZero = 0,
  .varOne = 0,
  .varTwo = 0,
  .changed = false
};

// select an initial mode.
enum {STA_MODE, AP_MODE};
int currentMode = STA_MODE;

// select intial ledstrip
int stripselect = WS2801;
// select an initial length.
int striplen = 1;

WiFiUDP OTA;
WiFiUDP effectListener;

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

String loadString(int& addr)
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
  for(int i = 0; i < 1024; i++)
  {
    storeInt(0, i);
  }
  int eeAddr = 0;
  storeString(board_name, eeAddr);
  storeString(sta_ssid, eeAddr);
  storeString(sta_pass, eeAddr);
  storeInt(accessPin, eeAddr);
  storeInt(stripselect, eeAddr);
  storeInt(striplen, eeAddr);
  // storeInt(currentMode, eeAddr);
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
  // currentMode = loadInt(eeAddr);
}

void handleSketchUpdate()
{
  if (OTA.parsePacket()) {
    IPAddress remote = OTA.remoteIP();
    int cmd  = OTA.parseInt();
    int port = OTA.parseInt();
    int size   = OTA.parseInt();

    Serial.print("Update Start: ip:");
    Serial.print(remote);
    Serial.printf(", port:%d, size:%d\n", port, size);
    uint32_t startTime = millis();

    WiFiUDP::stopAll();

    if(!Update.begin(size)){
      Serial.println("Update Begin Error");
      return;
    }

    WiFiClient client;
    if (client.connect(remote, port)) {

      uint32_t written;
      while(!Update.isFinished()){
        written = Update.write(client);
        if(written > 0) client.print(written, DEC);
      }
      Serial.setDebugOutput(false);

      if(Update.end()){
        client.println("OK");
        Serial.printf("Update Success: %u\nRebooting...\n", millis() - startTime);
        ESP.restart();
      } else {
        Update.printError(client);
        Update.printError(Serial);
      }
    } else {
      Serial.printf("Connect Failed: %u\n", millis() - startTime);
    }
  }
}

void findResponse(WiFiUDP listener)
{
  // create a packetd to put things in.
  listener.beginPacket(listener.remoteIP(), listener.remotePort());
  // reply with our board name.
  listener.print(board_name);
  Serial.print("Sending: ");
  Serial.println(board_name);
  Serial.print("to: ");
  Serial.print(listener.remoteIP());
  Serial.print(":");
  Serial.println(listener.remotePort());
  // end creating packet and send.
  listener.endPacket();
}

// ?pincode=1234&effect=0&brightness=255&var0=207&var1=255&var2=236
String variables[6][2];

String getAlphaNumString(String &data)
{
  String text = "";
  String tail = "";
  if(data[0] == '?' || data[0] == '=' || data[0] == '&')
  {
    int i = 0;
    while(isalnum(data[i+1]))
    {
      text += data[i+1];
      i++;
    }
    i++;
    while(data[i] != '\0')
    {
      tail += data[i];
      i++;
    }
  }
  data = tail;
  return text;
}

String effectArg(const char *par)
{
  for(int i = 0; i < 6; i++)
  {
    if(variables[i][0] == String(par))
    {
      return variables[i][1];
    }
  }
  return String("");
}

void applyEffectData()
{
  // don't apply if codes don't match.
  stripcontrol.pincode = effectArg("pin").toInt();
  if(accessPin != stripcontrol.pincode)
  {
    // debug print some info. 
    Serial.println("rejecting packet");
    Serial.println("pins: ");
    Serial.printf("%d:%d\n", accessPin, stripcontrol.pincode);
    return;
  }
  else
  {
    Serial.println("pins: ");
    Serial.printf("%d:%d\n", accessPin, stripcontrol.pincode);
    // parse and store in struct.
    stripcontrol.effect = effectArg("effect").toInt();
    stripcontrol.brightness = effectArg("brightness").toInt();
    stripcontrol.varZero = effectArg("var0").toInt();
    stripcontrol.varOne = effectArg("var1").toInt();
    stripcontrol.varTwo = effectArg("var2").toInt();
    stripcontrol.changed = true;
  }
  debugPrintStripControl();
}

void parseEffectPacket(String data)
{
  /* clear current settings. */
  for(int i = 0; i < 6; i++)
  {
    String parameter = "";
    String argument = "";
    variables[i][0] = parameter;
    variables[i][1] = argument;
  }
  /* apply received settings. */
  for(int i = 0; i < 6; i++)
  {
    String parameter = getAlphaNumString(data);
    String argument = getAlphaNumString(data);
    variables[i][0] = parameter;
    variables[i][1] = argument;
  }
}

void printPacketInfo(int packetSize)
{
  Serial.printf("packet size: %d ", packetSize);
  Serial.print("from: ");
  Serial.print(effectListener.remoteIP());
  Serial.printf(" port: %d\n", effectListener.remotePort());
}

String readPacketContents(WiFiUDP listener)
{
  String received = "";
  while(listener.available())
  {
    received += (char)listener.read();
  }
  return received;
}

void handleEffectUpdate()
{
  String received;
  int cb = effectListener.parsePacket();
  if(cb)
  {
    printPacketInfo(cb);
    if(effectListener.peek() == '?')
    {
      // got a http string.
      Serial.println("got effect string:");
      received = readPacketContents(effectListener);
      Serial.println(received);
      parseEffectPacket(received);
      applyEffectData();
    }
    else
    {
      Serial.println("got here");
      received = readPacketContents(effectListener);
      Serial.print("received: ");
      Serial.println(received);
      Serial.println("got a find request");
      Serial.print("received == EspFind: ");
      Serial.println(String("EspFind") == received ? "True":"False");
      if(String("EspFind") == received)
      {
        findResponse(effectListener);
      }
    }
  }
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
    /*acutally never gets here. but does if we allowed it to switch back.*/
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

  // setup wifi
  setupWifi(false);
  // enable OTA
  // Serial.setDebugOutput(true);
  OTA.begin(OTAPORT);

  Serial.println("done setting up pins, and WifiMode.");

  /* Setup server side things.*/
  server.on("/", handleRoot);
  server.on("/ledsettings", handleLedSettings);
  server.on("/wifisettings", handleWiFiSettings);

  server.begin();
  Serial.println("done setting up server");
  
  // start listening for udp packets. for effect controle.
  effectListener.begin(EFFECTPORT);

  // setup strips for the first time (initialize some pointers and stuff.)
  setupStrips(striplen);
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
