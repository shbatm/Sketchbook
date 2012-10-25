#include <PS2Keyboard.h>
PS2Keyboard keyboard;
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,8,7,6,5);

const int DataPin = 4;
const int IRQpin = 3;

byte text[33];
int Char = 0;
int y = 0;
int x = 0;
int Size = 32;
byte key;

void setup(){
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  lcd.begin(16,2);
  flushbuffer();
  Char = 0;
  lcdworks();
  Serial.println("Ready to type.");
}

void loop(){
  psinterface();
  lcdinterface();
}

void lcdinterface(){
  y=0;
  for(x=0;x<(Size/2);x++){
    lcd.setCursor(x,y);
    lcd.print(text[x]);
  }
  y=1;
  for(x=0;x<=(Size/2);x++){
    lcd.setCursor(x,y);
    lcd.print(text[x+16]);
  }
}

void psinterface(){
  if(keyboard.available()){
    key = keyboard.read();
    if(Char == 32){
      flushbuffer();
      Char = 0;
    }
    if(key == PS2_BACKSPACE){      
      text[Char] = ' ';
      Char--;
      printbuffer();
    }
    else if(key == PS2_ESC){
      flushbuffer();
      Char = 0;
    }
    else if(key == PS2_ENTER){
      flushbuffer();
      Char = 0;
    }
    else {
      text[Char] = key;
      Char++;
      printbuffer();
    }
  }
}

void printbuffer(){
  for(int i=0;i<=Size;i++){
    Serial.print(text[i]);
  }
  Serial.println(" ");
}

void flushbuffer(){
  for(Char =0;Char<=Size;Char++){
    text[Char] = 32;
    Serial.print(text[Char]);
  }
  Serial.println(" ");
}

void lcdworks(){
  lcd.setCursor(0,0);
  lcd.print("The lcd Works!");
  lcd.setCursor(0,1);
  lcd.print("2nd line works!");
  delay(1000);
  lcd.clear();
}
