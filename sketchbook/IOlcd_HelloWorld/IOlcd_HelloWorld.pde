#include <IOlcd.h>
#include <Wire.h>

IOlcd lcd;
int potpin = A3;
int potvalue;

void setup(){
  lcd.init(0x20,MCP23016);
  lcd.begin(16,2);
  pinMode(potpin, INPUT);
  lcd.clear();
  delay(100);
}

void loop(){
  potvalue = analogRead(potpin);
  lcd.home();
  lcd.print("0000");
  lcd.setCursor(0,0);
  lcd.print(potvalue);
  delay(1000);
}
