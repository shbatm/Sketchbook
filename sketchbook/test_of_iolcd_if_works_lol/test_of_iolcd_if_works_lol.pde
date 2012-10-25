#include <IOlcd.h>
#include <Wire.h>;

int i;
int x;
int y;
int dval;

IOlcd lcd;

void setup(){
  dval = 50;
  Serial.begin(115200);
  Serial.println("test");
  if(lcd.init(0x20, MCP23016))
    Serial.println("Communication with IOexpander works!");
  else
    Serial.println("No communication with the IOexpander!!");  
  
  lcd.begin(16,2,8);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("hello my name");
  lcd.setCursor(0,1);
  lcd.print("is Duality");
  lcd.blinkCursor();
  delay(3000);
  lcd.noBlinkCursor();
  delay(1000);
  lcd.blinkCursor();
  lcd.home();
  delay(1000);
  lcd.clear();
  Serial.print("type away");
  
}

void loop(){
  serialLcd();
//scroll();
//  for(i = 0;i<256;i++){
//    lcd.write(i);
//    delay(dval);
//    x++;
//    if(x == 16){
//      x = 0;
//      y = 1;
//    }
//    if(y == 1 && x == 15){
//      x = 0;
//      y = 0;
//      lcd.clear();
//      delay(dval);
//    }
//    lcd.setCursor(x,y);
//  }
}

void serialLcd(){
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    lcd.home();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      if (Serial.available() >= 16){
        lcd.setCursor(0,1);
      }
      lcd.write(Serial.read());
    }
  }
}

void scroll(){
  for(i=0;i<40;i++){
    lcd.scrollDisplayLeft();
    delay(200);
  }
  delay(1500);
  for(i>40;i--;){
    lcd.scrollDisplayRight();
    delay(200);
  }
  delay(1500);    
}
