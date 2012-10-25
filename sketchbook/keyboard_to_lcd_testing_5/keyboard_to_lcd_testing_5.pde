#include <LiquidCrystal.h>
#include <PS2Keyboard.h>
//initialize pins
LiquidCrystal lcd(12,11,8,7,6,5);
PS2Keyboard keyboard;

const int DataPin = 4;
const int IRQpin = 3;

int x = 0;
int y = 0;
int n;
int Size = 32;
int xval = 1000;
int charsperline[] = {0,0};
char text[32] = {0};
int Char = 0;
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
  lcdworks();
  for(Char=0;Char<=Size;Char++){
    text[Char] = 32;
  }
  Char = 0;
  Serial.println("Ready to type");
}

void loop(){
  if (keyboard.available()){
    key = keyboard.read();
    if(x==16 && y==1){
      y=0;
      x=0;
      for(Char=0;Char<=Size;Char++){
        text[Char] = 32;
      }
      Char=0;
      lcd.clear();
    }
    if((x == 16 && y == 0)||(x == 16 && y == 1)){
      if(!(y==1)){
        y=1;
      }
      else if(y==1){
        y=0;
      }
      if(!(x==0)){
        x=0;
      }
      else{
        x=0;
      }
    }
    if(key == PS2_ENTER){
      x=0;
      y=0;
      lcd.clear();
    }
    if(key == PS2_BACKSPACE && x>=1){
      x--;
      Char--;
      text[Char]=32;
      charsperline[0]--;
      lcd.print(" ");
      if(x==65535 && y==1){
        y=0;
        x=16;
      }
      lcd.setCursor(x,y);
      Serial.print(x*10+y);
      Serial.print(".");
      for(int i=0;i<=Size;i++){
        Serial.print(text[i]);
      }
      Serial.println(" ");
    }
    if (!(key == PS2_ENTER) && !(key == PS2_BACKSPACE)){      
      lcd.setCursor(x,y); 
      lcd.print(key);
      x++;
      text[Char] = key;
      Char++;
      charsperline[0]++;
      lcd.cursor();
      Serial.print(x*10+y);
      Serial.print(".");
      for(int i=0;i<=Size;i++){
        Serial.print(text[i]);
      }
      Serial.println(" ");
    }
  }
}

