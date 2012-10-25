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

int charsperline[] = {0,0};

void setup(){
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  Serial.begin(9600);
  Serial.println("Keyboard Available");
  lcd.setCursor(0,0);
  delay(500);
  lcd.print("Display");
  lcd.setCursor(0,1);
  lcd.print("Works");
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop(){
  readkeywritelcd();
}

void readkeywritelcd(){
  if(keyboard.available()){
    
    char c = keyboard.read();
    lcd.print(c);
    
    if(x == 16){
      y == 1;
      x == 0;
    }
    if(x == 16 && y == 1){
      y == 0;
      x == 0;
    }
    else if(!(c == PS2_ENTER) && !(PS2_BACKSPACE)){
      lcd.print(c);
      x = x +1;
    }
    lcd.setCursor(x,y);    
  }
}
