#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "html.h"
#include "stripcontrol.h"
#include "ws2801.h"

String ap_ssid = "TheEsp";
String ap_pass = "nospoonthereis";

String sta_ssid = "www.tkkrlab.nl";
String sta_pass = "hax4or2the2paxor3";

int accesPin = 1234;

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
int currentMode = AP_MODE;

int stripselect = WS2812;

WiFiUDP listener;

ESP8266WebServer server(80);

// stores a string into eeprom pluss nullterminator.
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
  ap ssid,
  ap pass,
  sta ssid,
  sta pass,
  accesPin,
  stripselect,
  currentMode
  */
  int eeAddr = 0;
  storeString(ap_ssid, eeAddr);
  storeString(ap_pass, eeAddr);
  storeString(sta_ssid, eeAddr);
  storeString(sta_pass, eeAddr);
  storeInt(accesPin, eeAddr);
  storeInt(stripselect, eeAddr);
  storeInt(currentMode, eeAddr);
  EEPROM.commit();
}

void settingsLoad()
{
  int eeAddr = 0;
  // Serial.println();
  // Serial.println(loadString(eeAddr));
  // Serial.println(loadString(eeAddr));
  // Serial.println(loadString(eeAddr));
  // Serial.println(loadString(eeAddr));
  // Serial.println(loadInt(eeAddr));
  // Serial.println(loadInt(eeAddr));
  // Serial.println(loadInt(eeAddr));
  ap_ssid = loadString(eeAddr);
  ap_pass = loadString(eeAddr);
  sta_ssid = loadString(eeAddr);
  sta_pass = loadString(eeAddr);
  accesPin = loadInt(eeAddr);
  stripselect = loadInt(eeAddr);
  currentMode = loadInt(eeAddr);
}

void handleSketchUpdate()
{
  int cb = listener.parsePacket();
  if(cb)
  {
    IPAddress remote = listener.remoteIP();
    int cmd  = listener.parseInt();
    int port = listener.parseInt();
    int sz   = listener.parseInt();
    Serial.println("Got update: ");
    Serial.printf("%d %d %d\r\n", cmd, port, sz);
    WiFiClient cl;
    if(!cl.connect(remote, port))
    {
      Serial.println("failed to connect.");
      ESP.reset();
    }
    listener.stop();
    if(!ESP.updateSketch(cl, sz))
    {
      Serial.println("Update failed.");
      ESP.reset();
    }
  }
}

String iptostr(IPAddress ip)
{
  String ipstr;
  for(int i = 0; i< 4; i++)
  {
    if(i == 3)
    {
      ipstr += String(ip[i]);
    }
    else
    {
      ipstr += String(ip[i]);
      ipstr += ".";
    }
  }
  return ipstr;
}

void moduleResetHandling(void)
{
  static unsigned long long buttoncount;
  static unsigned long long buttonprev;
  static int buttoninterval = 2000;

  buttoncount = millis();
  buttonprev = buttoncount;
  while(!digitalRead(0))
  {
    buttoncount = millis();
    if((buttoncount - buttonprev) >= buttoninterval)
    {
      pinMode(0, OUTPUT);
      digitalWrite(0, LOW);
      buttonprev = buttoncount;
      ESP.reset();
    }
    delay(1);
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
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
  Serial.println();
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

void setupSTA()
{
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
      currentMode == AP_MODE;
      setupAP();
      return;
    }
  }
  Serial.println();
  printWifiStatus();
}

void wifiModeHandling()
{
  if(!digitalRead(0))
  {
    moduleResetHandling();
    if(digitalRead(0))
    {
      if(currentMode == STA_MODE)
      {
        currentMode = AP_MODE;
        Serial.println("mode is now AP_MODE");
        setupAP();
      }
      else
      {
        currentMode = STA_MODE;
        Serial.println("mode is now STA_MODE");
        setupSTA();
      }
    }
  }
}

void setup() {
  // ESP.eraseConfig();
  Serial.begin(115200);
  EEPROM.begin(1024);
  settingsStore();
  settingsLoad();

  WiFi.disconnect();
  if(currentMode == STA_MODE)
  {
    currentMode = STA_MODE;
    setupSTA();
  }
  else
  {
    currentMode = AP_MODE;
    setupAP();
  }

  Serial.println("done setting up pins, and WifiMode.");

  server.on("/", handleRoot);
  server.on("/ledsettings", handleLedSettings);
  server.on("/wifisettings", handleWiFiSettings);
  server.on("/stripcontrol", handleStripControl);
  server.begin();
  Serial.println("done setting up server");

  // // updating related.
  Serial.setDebugOutput(true);
  listener.begin(8266);

  setupStrips();
}

void loop() {
  // handled in the stripcontrol handler
  // and here for now.
  server.handleClient();
  handleStrips();
  handleSketchUpdate();
  wifiModeHandling();
  yield();
}
