#include <IOlcd.h>
#include <Wire.h>;

IOlcd lcd;

void setup(){
  //initialize the chip(MCP23016) at address 0x20.
  lcd.init(0x20, MCP23016)
  //start the lcd
  lcd.begin(16,2);
  //start serial connection.
  Serial.begin(9800);
}

void loop(){
  // check if characters arrive over the serial port.
  if(Serial.available()){
    //wait a bit.
    delay(100);
    //clear the screen.
    lcd.clear();
    //read the data.
    while(Serial.available() > 0){
      //write data to the lcd.
      lcd.write(Serial.read());
    }
  }
}
