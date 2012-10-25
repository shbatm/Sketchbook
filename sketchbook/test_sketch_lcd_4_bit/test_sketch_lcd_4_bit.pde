#include <IOlcd.h>
#include <Wire.h>

IOlcd lcd;

void setup(){
  lcd.init(0x20, MCP23016);
  lcd.begin(16,2,8);
  lcd.blinkCursor();
  lcd.noBlinkCursor();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("Hello world");
}
