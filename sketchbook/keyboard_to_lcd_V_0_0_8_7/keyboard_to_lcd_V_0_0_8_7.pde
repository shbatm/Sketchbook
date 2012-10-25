#include <PS2Keyboard.h>
PS2Keyboard keyboard;
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,8,7,6,5);

const int DataPin = 4;
const int IRQpin = 3;

char text[32];
int index = 0;
int y = 0;
int x = 0;
int Size = 32;
byte key;

void setup(){
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(115200);
  lcd.begin(16,2);
  flushbuffer();
  index = 0;
  lcdworks();
  Serial.println("Ready to type.");
}

void loop(){
  psinterface();
  lcdinterface();
}

void psinterface(){
  if(keyboard.available()){  
    key = keyboard.read();
    if(index == Size){
      flushbuffer();
      index = 0;
    }
    if(key == PS2_BACKSPACE){
      text[index] = ' ';
      index--;      
      printbuffer();
    }
    else if(key == PS2_ESC){
      flushbuffer();
      index = 0;
      lcd.clear();
    }
    else if(key == PS2_ENTER){
      flushbuffer();
      index = 0;
    }
    else {
      text[index] = key;
      index++;
      text[index] = '\0';
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
  index = 0;
  text[index] = '/0';
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


void lcdworks(){
  lcd.setCursor(0,0);
  lcd.print("The lcd Works!");
  lcd.setCursor(0,1);
  lcd.print("2nd line works!");
  delay(1000);
  lcd.clear();
}
