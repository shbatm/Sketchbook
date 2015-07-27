#include "esputils.h"

// check for connection by checking for ip
void checkConnected()
{
  pinMode(0, INPUT_PULLUP);
  // wait wile we have not gotten an ip
  while(WiFi.status() != WL_CONNECTED)
  {
    // delay a bit is good for routines running in the background.
    delay(500);
    // show progress.
    Serial.print(".");
    // but if we have the wrong ssid + pass
    // we can just press the button and go in ap mode.
    if(!digitalRead(0))
    {
      setupAP();
      return;
    }
  }
  Serial.println();
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

// sets up the ap comfiguration.
void setupAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println();
  // print ip to know where we are on the network.
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

// sets up a station configuration.
void setupSTA()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  checkConnected();
  // print details so we know where we are on the network.
  printWifiStatus();
}
