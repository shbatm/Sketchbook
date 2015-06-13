#include "LedControl.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "WiFiUdp.h"
#include "Artnet.h"

LedControl lc = LedControl(13, 14, 4, 1);

Artnet artnet;


uint8_t ip[] = {192, 168, 2, 2};
uint8_t mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};

char ssid[] = "www.tkkrlab.nl";
char pass[] = "hax4or2the2paxor3";

MDNSResponder mdns;

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
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  printWifiStatus();
  if(!mdns.begin("esp8266", WiFi.localIP()))
  {
    Serial.println("Error setting up MDNS responder!");
    for(;;);
  }
  Serial.println("mDNS responder started");

  artnet.begin(mac, ip);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(artnet.read() == ART_DMX)
  {
    for(int i = 0;i < 8;i++)
    {
      lc.setRow(0, i, artnet.getDmxFrame()[i]);
    }
  }
}
