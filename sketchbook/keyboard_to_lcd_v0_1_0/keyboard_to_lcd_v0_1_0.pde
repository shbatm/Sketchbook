#include <IOlcd.h>
#include <Wire.h>
#include <PS2Keyboard.h>
#include <SD.h>

File savedInput;

IOlcd lcd;
PS2Keyboard keyboard;

const int DataPin = 3;
const int IRQpin = 2;

char Buffer[33];
int charposition = 0;
boolean ledstate;
int ledpin = 7;

char keyword[6] = "hello";

int newdata;

void setup(){
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  pinMode(10, OUTPUT);
  if(!SD.begin(4)){
    Serial.println("No connection to SD card.");
  }
  else{
    Serial.println("SD card initialized.");
  }
  if(lcd.init(0x20, MCP23016)){
    Serial.println("Lcd initialized.");
  }
  else{
    Serial.println("failed to initialize lcd screen.");
  }
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  lcd.home();
  lcd.clear();
  lcd.blinkCursor();
  emptyBuffer();
  Serial.println("Ready:");
}

void loop(){
  newdata = Keyboard();
  printBuffer();
  stringkeycheck();
}

int Keyboard(){
  if(keyboard.available()){
    char c = keyboard.read();
    switch(c){
      case PS2_BACKSPACE:
      charposition--;
      Buffer[charposition] = '\0';
      lcd.clear();
      if(charposition == -1){
        charposition = 0;
      }
      break;
      case PS2_ESC:
      emptyBuffer();
      charposition = 0;
      break;
      case PS2_ENTER:
      writeLnSD();
      emptyBuffer();
      charposition = 0;
      break;
      case PS2_F1:
      break;
      default:
      if(isprint(c)){
        Buffer[charposition] = c;
        charposition++;
      }
      break;
    }
    if(charposition==32){
      charposition = 0;
      emptyBuffer();
    }
    return 1;
  }
  else{
    return 0;
  }
}

void writeSD(){
  
}

void writeLnSD(){
  savedInput = SD.open("savedInput.txt", FILE_WRITE);
  
  if (!savedInput) {
    Serial.print("Writing to disk.");
    savedInput.println(Buffer);
	// close the file:
    savedInput.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }
}

void emptyBuffer(){
  for(int i=0;i<32;i++){
    Buffer[i] = '\0';
  }
  lcd.clear();
}

void printBuffer(){
   if(newdata == 1){
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
  Serial.print(Buffer);
  Serial.print("::");
  Serial.print(charposition);
  Serial.println(" ");
  }
  delay(100);
}
void stringkeycheck(){
  if(!strcmp(keyword, Buffer)){
    digitalWrite(ledpin, HIGH);
  }
  else{
    digitalWrite(ledpin, LOW);
  }
}
