//include librarys
#include <LiquidCrystal.h>
#include <PS2Keyboard.h>
//initialize the library interface pins
LiquidCrystal lcd(12,11,8,7,6,5);
PS2Keyboard keyboard;

const int DataPin = 4;
const int IRQpin = 3;
int x;
int x1;
int xp;
int y;
int n;
int charsperline[] = {0,0};

void setup(){
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  Serial.begin(9600);
  Serial.println("Keyboard Available");
  delay(1000);
}

void loop(){
  if(keyboard.available()){
    char c = keyboard.read();
    if(x>=16){
      lcd.scrollDisplayLeft();
    }
    if(x==40 && y==0){
      y=1;
      x=0;
      x1=0;
    }
    if(x==40 && y==1){
      y=0;
      x=0;
    }
    if(c == PS2_ENTER){
      y=1;
      x=0;
      x1=16;
    }
    if(c == PS2_BACKSPACE && x>0){
      x--;
      lcd.setCursor(x,y);
      charsperline[0]--;
      lcd.print(" ");
      if(x==0 && y==1){
        y=0;
        x=charsperline[n];
      }
    }
    if(c == PS2_ESC){
      lcd.clear();
      y=0;
      x=0;
      x1=0;
    }
    else if(!(c == PS2_ENTER) && !(c == PS2_BACKSPACE)){
      lcd.print(c);
      charsperline[0]++;
      x++;
    }
    lcd.setCursor(x,y);
    lcd.cursor();
  }
  if(x1<16 && y==1){
    x1++;
    lcd.scrollDisplayLeft();
  }
}


