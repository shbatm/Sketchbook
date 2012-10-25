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
char noSDcard[22] = "No SD card in holder!";

int newdata;
int x;
int y;

void setup(){
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, INPUT);
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
  Serial.println("assuming keyboard initialized.");
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  lcd.home();
  lcd.clear();
  lcd.blinkCursor();
  emptyBuffer();
  Serial.println("Ready:");
  lcd.print("Ready:");
  delay(500);
  lcd.clear();
  lcd.home();
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
      emptyBuffer();
      charposition = readLnSD(0);
      Serial.println("readLnSD is done?");
      lcd.clear();
      Serial.println(c);
      break;
      case PS2_F2:
      Serial.print(c);
      break;
      default:
      if(isprint(c)){
        Buffer[charposition] = c;
        charposition++;
      }
      break;
    }
    if(charposition==32){
      writeLnSD();
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

int readLnSD(int charplace){
  int i = charplace;
  savedInput = SD.open("test.txt");
  if(savedInput){
    Serial.println("Reading from disk.");
    while(savedInput.available()){
      char c = savedInput.read();
      if(c != '\n' && c != '\r'){
        Buffer[i]=c;
        i++;
      }
    }
    savedInput.close();
  }
  else{
    Serial.println("Error getting data.");
  }
  Serial.println("done");
  return i;
}

void writeLnSD(){
  savedInput = SD.open("test.txt", FILE_WRITE);
  
  if (savedInput) {
    Serial.print("Writing to disk.");
    savedInput.print(Buffer);
    savedInput.println('\n');
	// close the file:
    savedInput.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file on SD card.");
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
    Serial.print("||");
    Serial.print("::");
    Serial.print("||");
    Serial.print(charposition);
//    for(int i=0;i<32;i++){
//      Serial.print(Buffer[i], HEX);
//      Serial.print(".");
//    }
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
