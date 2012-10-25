#include <IOlcd.h>
#include <Wire.h>

IOlcd lcd;

//here we create an array that contains the actual data that's
//going to display a smiley, you see it even looks like it.
byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};


void setup() {
  //it's important to always first initialize the IO chip.
  lcd.init(0x20, MCP23016);
  //and it's also imporant to create the char before you even 
  //start the lcd.
  lcd.createChar(0, smiley);
  //here we start the lcd like normal
  lcd.begin(16, 2);
  //we set cursor "home", and clearing maybe previous rubish on screen
  lcd.home();
  lcd.clear();
  //we write the costum char. first int in createChar is memory addres.
  //when you write that memory address with lcd.write you'l print it on 
  // the lcd.
  lcd.write(0);
}

void loop() {}

