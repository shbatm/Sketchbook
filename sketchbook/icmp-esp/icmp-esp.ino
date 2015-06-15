#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServer.h>

const char* ap_ssid = "TheEsp";
const char* ap_password = "nospoonthereis";

const char* ssid = "wwww.tkkrlab.nl";
const char* password = "hax4or2the2paxor3";

ESP8266WebServer server(80);

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

void handleRoot(){}

void setupAP()
{
  WiFi.mode(WIFI_AP);
}

void setupSTA()
{
  WiFi.mode(WIFI_STA);
}

void setup() {
  Serial.begin(115200);
}

void loop() {
}
