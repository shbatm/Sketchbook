//include the nesecary header files for this to work.
#include <IOlcd.h>
#include <Wire.h>;

IOlcd lcd;
long count = 1;

void setup(){
  //initialize the chip(in this case MCP23016) at address 0x20
  lcd.init(0x20, MCP23016);
  //start the lcd
  lcd.begin(16,2);
  //at the begining of the first line.
  lcd.home();
  //clear lcd just incase.
  lcd.clear();
  //print hello world to the first line.
  lcd.print("Hello, World!");
}

void loop(){
  //set cursor to the second line.
  lcd.setCursor(0,1);
  //number of seconds active, is printed.
  lcd.print(count);
  count++;
  delay(1000);
}
