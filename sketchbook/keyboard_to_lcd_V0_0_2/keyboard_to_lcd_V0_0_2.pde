
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 8, 7, 6, 5);

#include <PS2Keyboard.h>

const int DataPin = 4;
const int IRQpin = 3;
int x;  //x position on screen
int y;  //y position on screen

PS2Keyboard keyboard;

void setup() {
 delay(1000);
 keyboard.begin(DataPin, IRQpin);
 lcd.begin(16,2);
 Serial.begin(9600);
 Serial.println("keyboard available?");
}

void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    if(c == PS2_ENTER){
      y=1;
      x=0;
      lcd.setCursor(x,y);          
    }
    if(x>=15){
      if(!(c == PS2_BACKSPACE)){
      lcd.scrollDisplayLeft();
      }
    }
    if(c == PS2_BACKSPACE && x>0){
      x--;
      lcd.setCursor(x,y);
      lcd.print(" ");
      
      if(x>15){
      lcd.scrollDisplayRight();
      }
      if(x==0 && y==1){
        y=0;
        x=15;
      }
      
      lcd.setCursor(x,y);      
    }
    else if(!(c == PS2_ENTER)){
      lcd.print(c);
      x++;
    }
    lcd.cursor();
  }
}

