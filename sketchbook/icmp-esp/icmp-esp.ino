#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServer.h>

char ap_ssid[] = "TheEsp";
char ap_password[] = "nospoonthereis";

char ssid[] = "www.tkkrlab.nl";
char pass[] = "hax4or2the2paxor3";

enum {STA_MODE, AP_MODE};
int currentMode = AP_MODE;

ESP8266WebServer server(80);
#include "htmlroot.h"

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
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println();
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("done setting up server");
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

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT);
  pinMode(0, INPUT_PULLUP);
  setupAP();
}

void loop() {
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
  server.handleClient();
}
