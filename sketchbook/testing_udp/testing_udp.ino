#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

char ssid[] = "pass";
char pass[] = "pass";

#define ARTNETPORT 6454

char packetBuffer[800];

WiFiUDP Udp;

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
  Serial.begin(115200);

  // attempt connecting.
  Serial.print("\nAttempting to connect to SSID: ");
  Serial.println(ssid);
  while(WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(500);
  }
  Serial.println("connected to wifi.");
  printWifiStatus();
}

void loop() {
  // put your main code here, to run repeatedly:

}
