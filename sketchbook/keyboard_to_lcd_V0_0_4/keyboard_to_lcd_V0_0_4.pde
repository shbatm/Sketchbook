//include librarys
#include <LiquidCrystal.h>
#include <PS2Keyboard.h>
//initialize the library interface pins
LiquidCrystal lcd(12,11,8,7,6,5);
PS2Keyboard keyboard;

const int DataPin = 4;
const int IRQpin = 3;
int x;
int y;
int n;
int charsPerLine[] = {0, 0};

void setup(){
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  Serial.begin(9600);
  Serial.println("Keyboard Available?");
  delay(1000);
}

void loop(){
  if(keyboard.available()){
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
      charsPerLine[0]--;
      lcd.setCursor(x,y);
      lcd.print(" ");
      if(x>15){
        lcd.scrollDisplayRight();
      }
      if(x==0 && y==1){
        y=0;
        x=charsPerLine[n];
      }
      lcd.setCursor(x,y);
    }
    if(x==40 && y==0){
      y=1;
      x=0;
      lcd.setCursor(x,y);
    }
    if(x==40 && y==1){
      y=0;
      x=0;
      lcd.clear();
      lcd.setCursor(x,y);
    }
    else if(!(c == PS2_ENTER) && x<40){
      lcd.print(c);
      charsPerLine[0]++;
      x++;
    }
    lcd.cursor();
  }
}
