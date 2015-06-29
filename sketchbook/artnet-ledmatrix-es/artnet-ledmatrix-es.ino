#include "LedControl.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "WiFiUdp.h"
#include "Artnet.h"

LedControl lc = LedControl(13, 14, 4, 1);

Artnet artnet;

uint8_t ip[] = {192, 168, 2, 2};
uint8_t mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};

char ssid[] = "www.tkkrlab.nl";
char pass[] = "hax4or2the2paxor3";

MDNSResponder mdns;
WiFiUDP listener;

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

void setupAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  Serial.println();
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

void setupSTA()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  printWifiStatus();
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

void setup() {
  // setup max7219
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
  // serial for debugging.
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  setupSTA();
  // listener for updating.
  listener.begin(8266);
  // start artnet.
  artnet.begin(mac, ip);
}

void loop() {
  handleSketchUpdate();
  if(artnet.read() == ART_DMX)
  {
    for(int i = 0;i < 8;i++)
    {
      // data is in byte format so easy as this..
      lc.setRow(0, i, artnet.getDmxFrame()[i]);
    }
  }
}
