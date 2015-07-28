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

int accesPin = 0;

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

int stripselect = WS2812;

WiFiUDP listener;

ESP8266WebServer server(80);

int storeString(String string, int addr)
{
  const char *str = string.c_str();
  int str_len = strlen(str);
  int i;
  for(i = 0; i < str_len; i++)
  {
    EEPROM.write(addr+i, str[i]);
  }
  i++;
  EEPROM.write(addr+i, '\0');
  return i;
}

int storeInt(int value, int addr)
{
  int i;
  for(i = 0; i < sizeof(value); i++)
  {
    EEPROM.write(addr+i, (value>>(i*8))&0xFF);
  }
  return i;
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
  eeAddr += storeString(ap_ssid, eeAddr);
  eeAddr += storeString(ap_pass, eeAddr);
  eeAddr += storeString(sta_ssid, eeAddr);
  eeAddr += storeString(sta_pass, eeAddr);
  eeAddr += storeInt(accesPin, eeAddr);
  eeAddr += storeInt(stripselect, eeAddr);
  eeAddr += storeInt(currentMode, eeAddr);
  EEPROM.commit();
}

void settingsLoad()
{

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
      for(;;);
    }
    delay(0);
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
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
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
  Serial.begin(115200);
  EEPROM.begin(1024);
  settingsStore();
  pinMode(0, INPUT_PULLUP);
  setupSTA();
  Serial.println("done setting up pins, and WifiMode.");

  server.on("/", handleRoot);
  server.on("/ledsettings", handleLedSettings);
  server.on("/wifisettings", handleWiFiSettings);
  server.on("/stripcontrol", handleStripControl);
  server.begin();
  Serial.println("done setting up server");

  // updating related.
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
}
