#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);
MDNSResponder mdns;

int n = 0;
String ssids;

void handle_root()
{
  server.send(200, "text/plain", "Welcome there!");
}

void handle_wifi_page()
{  
  String encription = String("* == No Encrip <br>");
  String numsAround = String("Num of networks around: " + String(n) + "<br>");
  
  String page = "<META HTTP-EQUIV=\"refresh\" CONTENT=\"15\">"+encription + numsAround + ssids;
  String head = "<head> <META HTTP-EQUIV=\"refresh\" CONTENT=\"1\"> </head>";
  String body = "<body>" + page + "</body";
  String html = "<html>" + head + body +"</html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  
  WiFi.begin("www.tkkrlab.nl", "hax4or2the2paxor3");
  Serial.println("\nConnecting.");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi connected.");
  server.on("/", handle_root);
  server.on("/wifi", handle_wifi_page);
  server.on("/inline", [] (){
    server.send(200, "text/plain", "this works as well");
  });
  Serial.println(WiFi.localIP());
  if(!mdns.begin("esp8266-Duality", WiFi.localIP()))
  {
    Serial.println("Error setting up MDNS/DNS responder.");
    while(1);
  }
  Serial.println("dns setup ");
  Serial.println("esp8266-Duality");
  server.begin();
  Serial.println("exiting setup: ");
}

unsigned long current = millis();
unsigned long previous = 0;
int interval = 1000;

void getWifiScan()
{
  current = millis();
  if(current - previous > interval)
  {
    previous = current;
    n = WiFi.scanNetworks();
    ssids = String("");
    for(int i = 0; i < n; ++i)
    {
      ssids += (String(WiFi.SSID(i))+" ");
      ssids += (String(WiFi.RSSI(i))+" ");
      ssids += (String(WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      ssids += String("<br>");
    }
  }
}

void loop() {
  server.handleClient();
  getWifiScan();
}

