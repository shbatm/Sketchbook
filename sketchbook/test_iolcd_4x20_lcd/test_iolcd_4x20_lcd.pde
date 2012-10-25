#include <IOlcd.h>
#include <Wire.h>
IOlcd lcd;
int i = 0;
int y = 0;
void setup(){
  lcd.init(0x20, MCP23016);
  lcd.begin(20,4);
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("teehee");
  lcd.setCursor(0,1);
  lcd.print("teehee two");
  lcd.setCursor(0,2);
  lcd.print("third line!");
  lcd.setCursor(0,3);
  lcd.print("fourth line!");
  delay(1000);  
}

void loop(){
}
