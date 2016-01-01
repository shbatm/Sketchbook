/*
This is a basic example that will print out the header and the content of an ArtDmx packet.
This example uses the read() function and the different getter functions to read the data.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "WiFiUdp.h"
#include "Artnet.h"
#include <SPI.h>

Artnet artnet;

// Change ip and mac address for your setup
uint8_t ip[] = {192, 168, 2, 2};
uint8_t mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};
char ssid[] = "ssid"; //  your network SSID (name)
char pass[] = "pass";    // your network password (use for WPA, or use as key for WEP)

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

void setup()
{
  Serial.begin(115200);
  Serial.println();

  
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  printWifiStatus();
  if (!mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  
  artnet.begin(mac, ip);
}

void loop()
{
  if (artnet.read() == ART_DMX)
  {
    // print out our data
    Serial.print("universe number = ");
    Serial.print(artnet.getUniverse());
    Serial.print("\tdata length = ");
    Serial.print(artnet.getLength());
    Serial.print("\tsequence n0. = ");
    Serial.println(artnet.getSequence());
    Serial.println("DMX data: ");
    for (int i = 0 ; i < 64 ; i++)
    {
      Serial.write(artnet.getDmxFrame()[i]);
    }
    Serial.println();
  }
}

