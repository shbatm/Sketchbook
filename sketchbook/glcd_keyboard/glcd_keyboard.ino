
#define byte_type uint8_t
#define buffsize 50
byte_type textbuffer[buffsize];
int buffindex;

//IO input (keyboard) part:
char command = 0;

#include <PS2Keyboard.h>
const int DataPin = 12;
const int IRQpin = 3;
PS2Keyboard key;

//glcd library. displaying part.
#include <glcd.h>
#include "fonts/allFonts.h"

glcd lcd;

gText textLEFT = gText(textAreaLEFT);
gText textRIGHT = gText(textAreaRIGHT);

void setup(){
  startKeyboard();
  startGlcd();
}

void loop(){
  //typing and displaying and processing command/input from keyboard.
  if(recieveData(textbuffer)==1){
    sendData(textbuffer, 1, 'l', command);
  }
  menu();
  //for debug on right halve of the screen.
  textRIGHT.CursorTo(0,3);
  textRIGHT.println("pressed enter");
  textRIGHT.CursorTo(0,0);
  textRIGHT.println("command: ");
  textRIGHT.println(command, DEC);
  textRIGHT.CursorTo(3,2);
  textRIGHT.print(millis());
}

void menu(){
  
}

void startKeyboard(){
  key.begin(DataPin, IRQpin);
};

void startGlcd(){
  //glcd
  lcd.Init();
  lcd.SelectFont(System5x7);
  lcd.ClearScreen();
  //setup for left text area:
  textLEFT.SelectFont(System5x7);
  textLEFT.CursorTo(0,0);
  //setup for right textArea:
  textRIGHT.SelectFont(System5x7);
  textRIGHT.CursorTo(0,0);
};

void sendData(byte_type* tempBuffer, int type, char Side, char Command){
  if(Side=='l'){
    textLEFT.ClearArea();
  }
  else if(Side == 'r'){
    textRIGHT.ClearArea();
  }
  for(int tempIndex = 0;tempIndex<strlen((char*)tempBuffer);tempIndex++){
    if(tempBuffer[tempIndex]=='\n'){
      if(Side == 'l'){
        textLEFT.write('\r\n');
      }
      else  if(Side=='r'){
        textRIGHT.write('\r\n');
      }
    }
    else if(tempBuffer[tempIndex]=='\0'){
      break;
    }
    switch(type){
      case 0:
      if(Side=='l'){
        textLEFT.print(tempBuffer[tempIndex]);
      }
      else if(Side=='r'){
        textRIGHT.print(tempBuffer[tempIndex]);
      }
      break;
      case 1:
      if(Side=='l'){
        textLEFT.print((char)tempBuffer[tempIndex]);
      }
      else if(Side=='r'){
        textRIGHT.print((char)tempBuffer[tempIndex]);
      }
      break;
      case 2:
      if(Side=='l'){
        textLEFT.print(tempBuffer[tempIndex], HEX);
      }
      else if(Side=='r'){
        textRIGHT.print(tempBuffer[tempIndex], HEX);
      }
      break;
      case 3:
      if(Side=='l'){
        textLEFT.print(tempBuffer[tempIndex], BIN);
      }
      else if(Side=='r'){
        textRIGHT.print(tempBuffer[tempIndex], BIN);
      }
      break;
    }
  }
};

byte_type recieveData(byte_type* tempBuffer){
  if(key.available()){
    while(key.available()){
      byte_type inkey = key.read();
      if(inkey>=32 && inkey <=126||inkey==13){
        if(inkey == 13){
          tempBuffer[buffindex]='\n';
        }else{
          tempBuffer[buffindex]=inkey;
        }
        tempBuffer[buffindex+1]='\0';
        buffindex++;
        if(buffindex+1==buffsize){
          buffindex = 0;
          break;
        }
      }else{
        command = inkey;
      }
    }
    return 1;
  }else{
    return 0;
  }
};
