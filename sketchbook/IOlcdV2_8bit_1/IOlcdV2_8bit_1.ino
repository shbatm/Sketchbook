#include <IOlcd.h>
#include <Wire.h>;

IOlcd lcd(0x20);

void setup(){
  lcd.begin(16,2);
}
void loop(){}
