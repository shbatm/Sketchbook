#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServer.h>

const char* ap_ssid = "TheEsp";
const char* ap_password = "nospoonthereis";

const char* ssid = "wwww.tkkrlab.nl";
const char* password = "hax4or2the2paxor3";

ESP8266WebServer server(80);

void resetModule(void)
{
  ESP.reset();
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

void handleRoot()
{
  String html = 
  "<h1>"
    "Hello there!"
  "</h1>"
  ;
  server.send(200, "text/html", html);
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
}

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  setupAP();
}

unsigned long long buttoncount;
unsigned long long buttonprev;
int buttoninterval = 2000;

void loop() {
  if(!digitalRead(0))
  {
    buttoncount = millis();
    buttonprev = buttoncount;
    while(!digitalRead(0))
    {
      buttoncount = millis();
      if((buttoncount - buttonprev) >= buttoninterval)
      {
        buttonprev = buttoncount;
        resetModule();
      }
      delay(0);
    }
  }
  server.handleClient();
}
