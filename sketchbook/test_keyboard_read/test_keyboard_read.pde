#include <IOlcd.h>
#include <Wire.h>
#include <PS2Keyboard.h>

IOlcd lcd;
PS2Keyboard keyboard;

const int DataPin = 3;
const int IRQpin = 2;

char Buffer[33];
int charposition = 0;
char c;

void setup(){
  lcd.init(0x20, MCP23016);
  lcd.begin(16,2);
  lcd.home();
  lcd.clear();
  lcd.blinkCursor();
  emptyBuffer();
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(115200);
}

void loop(){
  if(keyboard.available()){
    Keyboard();
    printBuffer();
    Serial.println(Buffer);
  }
}

void Keyboard(){
  c = keyboard.read();
  switch(c){
    case PS2_ESC:
    emptyBuffer();
    charposition = 0;
    break;
    case PS2_ENTER:
    if(charposition > 16){
      emptyBuffer();
      charposition = 0;
    }
    else{
      charposition = 16;
    }
    break;
    default:
    if(c != PS2_ENTER && c != PS2_ESC){
      Buffer[charposition] = c;
      charposition++;
    }
    break;
  }
  if(charposition==32){
    charposition = 0;
    emptyBuffer();
  }
}

void emptyBuffer(){
  for(int i=0;i<32;i++){
    Buffer[i+1] = '\0';
  }
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0);
}

void printBuffer(){
  for(int i=0;i<32;i++){
    if(i>=16 && Buffer[i]!='\0'){
      lcd.setCursor((i-16),1);
      lcd.print(Buffer[i]);
    }
    else if(Buffer[i]!='\0'){
      lcd.setCursor(i,0);
      lcd.print(Buffer[i]);
    }
  }
}

