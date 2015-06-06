#include <ESP8266WiFi.h>
#include "WiFiUdp.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
  WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort

 Circuit:
 * WiFi shield attached

 created 30 December 2012
 by dlf (Metodo2 srl)

 */
 
char ssid[] = "www.tkkrlab.nl"; //  your network SSID (name)
char pass[] = "hax4or2the2paxor3";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 6454;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

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

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  // attempt to connect to Wifi network:
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("\nAttempting to connect to SSID: ");
    Serial.println(ssid);
    // wait 10 seconds for connection:
    delay(500);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  
  Wire.pins(0, 2);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
}

//unsigned long current = millis();
//unsigned long previous = 0;
//int interval = 100;

char buff[123];

bool extractUdpData(char *buff)
{
  Udp.parsePacket();
  if(Udp.available() > 0)
  {
    int max = Udp.available();
    for(int i = 0; i != max; i++) 
    {
      buff[i] = Udp.read();
    }
    Udp.flush();
    return max;
  }
  return 0;
}

void displayChar(char *data)
{
  byte newChar[8];
  int offset = 10;
  for(int i = 0; i < 8;i++)
  {
    newChar[i] = data[i+offset];
    Serial.write(newChar[i]);
  }
  lcd.createChar(0, newChar);
  lcd.setCursor(0, 1);
  lcd.write((uint8_t)0);
}

void loop() {

  // if there's data available, read a packet
  //int packetSize = Udp.parsePacket();
  lcd.setCursor(0, 0);
  lcd.print(millis());
  if(extractUdpData(buff))
    displayChar(buff);
}

