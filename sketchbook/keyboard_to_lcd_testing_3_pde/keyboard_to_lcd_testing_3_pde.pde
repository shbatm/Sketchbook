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
int i;
int Size = 8;
int xval = 1000;
int charsperline[] = {0,0};
char text[] = {0,0,0,0,0,0,0,0};
int Char;
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
  for(i=0;i<=Size;i++){
    text[i] = 32;
  }
}

void loop(){
  if (keyboard.available()){
    key = keyboard.read();
    if(x==16 && y==1){
      y=0;
      x=0;
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
    if(key == PS2_BACKSPACE){
      x--;
      Char--;
      text[Char]=32;
      charsperline[0]--;
      lcd.print(" ");
      if(x==65535 && y==1){
        y=0;
        x=charsperline[n];
      }
      lcd.setCursor(x,y);
    }
    if (!(key == PS2_ENTER) && !(key == PS2_BACKSPACE)){      
      lcd.setCursor(x,y); 
      lcd.print(key);
      x++;
      text[Char] = key;
      Char++;
      charsperline[0]++;
      lcd.cursor();
      Serial.println(x*10+y);
      for(i=0;i<=Size;i++){
        Serial.print(text[i]);
      }
    }
  }
}

