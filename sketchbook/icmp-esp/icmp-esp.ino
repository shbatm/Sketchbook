#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "Artnet.h"
#include "htmlroot.h"

char ap_ssid[] = "TheEsp";
char ap_password[] = "nospoonthereis";

char ssid[] = "HuisVanDerTuuk";
char pass[] = "10SamSung@H";

char mac[] = {'e','s','p','l','c','0'};

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

enum {STA_MODE, AP_MODE};
int currentMode = STA_MODE;

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

ESP8266WebServer server(80);

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
  Serial.setDebugOutput(true);
  pinMode(0, INPUT);
  pinMode(0, INPUT_PULLUP);
  setupSTA();

  server.on("/", handleRoot);
  server.on("/ledsettings", handleLedSettings);
  server.on("/wifisettings", handleWiFiSettings);
  server.begin();
  Serial.println("done setting up server");

  // updating related.
  listener.begin(8266);
}

void loop() {
  handleSketchUpdate();
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
