#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "www.tkkrlab.nl";
const char* pass = "hax4or2the2paxor3";

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
    }
    listener.stop();
    if(!ESP.updateSketch(cl, sz))
    {
      Serial.println("Update failed.");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  listener.begin(8266);
}

unsigned long previous = 0;
unsigned long current = 0;
int interval = 1000;
bool shouldSend = false;

void loop()
{
  // handleSketchUpdate();
  if(shouldSend)
  {
    Serial.println("Hello!");
  }
  current = millis();
  if((current - previous) >= interval)
  {
    previous = millis();
    shouldSend = !shouldSend;
  }
}

