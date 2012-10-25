#include <IOlcd.h>
#include <Wire.h>;

IOlcd lcd;
int x;
int y;

void setup(){
  Serial.begin(115200);
  if(lcd.init(0x20, MCP23016)){
    Serial.println("Communication with IOlcd works!");
  }
  else{
    Serial.println("Not connected propperly!");
  }
  lcd.begin(16,2,8);
  lcd.clear();
}

void loop(){
  if(Serial.available()){
    delay(100);
    lcd.clear();
    x=0;
    y=0;    
    while(Serial.available() > 0){
      if(x == 15){
        x=0;
        y=1;
      }
      else if(x == 16 && y == 1){
        x=0;
        y=0;
      }
      lcd.setCursor(x,y);
      Serial.print(x);
      Serial.println(" ");
      lcd.write(Serial.read());
      x++;
    }
  }
}
