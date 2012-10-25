#include <Wire.h>;
#include <IOlcd.h>

IOlcd lcd;

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void setup(){
  lcd.begin(20,4,0x20);
  lcd.createChar(0, heart);
  //for(int i=0;i<255;i++){
  //  lcd.write(i);
  //}
  lcd.clear();
  lcd.blinkCursor();
  lcd.setCursor(0,0);
  lcd.print("hello world!! ");
  lcd.setCursor(0,1);
  lcd.print("second line!!");
  lcd.setCursor(14,1);
  lcd.write(0);
  
}
void loop(){}
