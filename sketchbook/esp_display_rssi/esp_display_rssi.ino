#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ESP8266WiFi.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const String ssid = "www.tkkrlab.nl";

void setup() {
  // put your setup code here, to run once:
  Wire.pins(0, 2);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.home();
  lcd.clear();
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int n = WiFi.scanNetworks();
  for(int i = 0; i < n;i++)
  {
    if(String(WiFi.SSID(i)) == ssid)
    {
      lcd.setCursor(0, 0);
      lcd.print(WiFi.SSID(i));
      lcd.setCursor(0, 1);
      lcd.print(WiFi.RSSI(i));
    }
  }
}
