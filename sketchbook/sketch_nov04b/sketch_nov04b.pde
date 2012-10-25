#include <LiquidCrystal.h>
#include <PS2Keyboard.h>
//initialize pins
LiquidCrystal lcd(12,11,8,7,6,5);
PS2Keyboard keyboard;

const int DataPin = 4;
const int IRQpin = 3;

int x = 0;
int y = 0;
int k = 0;
int xval = 1000;
int charsperline[] = {0,0};
char key;

void lcdworks(){
  lcd.setCursor(0,0);
  lcd.print("liquidCrystal");
  lcd.setCursor(0,1);
  lcd.print("available??");
  delay(xval);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Display");
  lcd.setCursor(0,1);
  lcd.print("Works");
  delay(xval);
  lcd.clear();
  lcd.setCursor(0,0);
}

void setup(){
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  Serial.begin(9600);
  Serial.println("keyboard available?");
  lcdworks();
}

void loop(){
  if (keyboard.available()){
    key = keyboard.read();
    
    if(x == 16 && y == 0){
      x=0;
      y=1;
    }
    if (!(key == PS2_ENTER) && !(key == PS2_BACKSPACE)){ 
      lcd.print(key);
      x++;
      lcd.cursor();
      Serial.println(x*10+y);
    }
    lcd.setCursor(x,y);
  }
}

