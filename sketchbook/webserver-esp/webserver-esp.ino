#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "html.h"
#include "stripcontrol.h"

#define AP_BUTTON 0

String board_name = "TkkrEsp-01";

String sta_ssid = "www.tkkrlab.nl";
String sta_pass = "hax4or2the2paxor3";
// String sta_ssid = "HuisVanDerTuuk";
// String sta_pass = "10SamSung@H";

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

enum {STA_MODE, AP_MODE};
int currentMode = STA_MODE;

int stripselect = WS2801;
int striplen = 1;

WiFiUDP uploadListener;
WiFiUDP effectListener;

ESP8266WebServer server(80);

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
  storeInt(currentMode, eeAddr);
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
  currentMode = loadInt(eeAddr);
}

void handleSketchUpdate()
{
  int cb = uploadListener.parsePacket();
  if(cb)
  {
    IPAddress remote = uploadListener.remoteIP();
    int cmd  = uploadListener.parseInt();
    int port = uploadListener.parseInt();
    int sz   = uploadListener.parseInt();
    Serial.println("Got update: ");
    Serial.printf("%d %d %d\r\n", cmd, port, sz);
    WiFiClient cl;
    if(!cl.connect(remote, port))
    {
      Serial.println("failed to connect.");
      ESP.reset();
    }
    uploadListener.stop();
    if(!ESP.updateSketch(cl, sz))
    {
      Serial.println("Update failed.");
      ESP.reset();
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
  WiFi.disconnect();
  currentMode = AP_MODE;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(board_name.c_str());
  Serial.println();
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

void setupSTA()
{
  currentMode = STA_MODE;
  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());
  int i = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    i++;
    if(i == 10)
    {
      currentMode = AP_MODE;
      setupAP();
      return;
    }
  }
  Serial.println();
  printWifiStatus();
}

void wifiModeHandling()
{
  if(!digitalRead(AP_BUTTON))
  {
    Serial.println("got here");
    if(currentMode == STA_MODE)
    {
      Serial.println("mode is now AP_MODE");
      setupAP();
    }
    else
    {
      Serial.println("mode is now STA_MODE");
      setupSTA();
    }
    while(!digitalRead(AP_BUTTON)) delay(50);
  }
}

void setup() {
  Serial.begin(115200);
  // prepare eeprom for use.
  EEPROM.begin(1024);
  // settingsStore();
  settingsLoad();
  // updating related.
  Serial.setDebugOutput(true);
  uploadListener.begin(8266);

  pinMode(AP_BUTTON, INPUT_PULLUP);

  WiFi.disconnect();
  // start connecting. will switch to Access Point
  // if no connections can be made.
  setupSTA();

  Serial.println("done setting up pins, and WifiMode.");

  /* Setup server side things.*/
  server.on("/", handleRoot);
  server.on("/ledsettings", handleLedSettings);
  server.on("/wifisettings", handleWiFiSettings);

  /* used to be used for controlling with app.*/
  // server.on("/stripcontrol", handleStripControl);

  server.begin();
  Serial.println("done setting up server");
  
  // start listening for udp packets.
  effectListener.begin(1337);

  setupStrips(striplen);
}

void loop() {
  if((currentMode == AP_MODE) || 1)
  {
    // only serve pages in Access point mode.
    server.handleClient();
  }
  if(currentMode == STA_MODE)
  {
    // enable ledstrip animations.
    handleStrips();
    // enable controle over effects.
    handleEffectUpdate();
    // enable switching to AP_MODE
    wifiModeHandling();
  }
  // always be able to upload new firmware <dev>
  handleSketchUpdate();
}
