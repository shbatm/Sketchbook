#include <IOlcd.h>
#include <Wire.h>
#include <PS2Keyboard.h>

int MenuIndex=1;
char Menu[9] [17] = {
  {"Select Program: "},
  {"1.Serial Monitor"},
  {"2. Test entry   "},
};
uint8_t initialSettings[6] [17]={
  {"Modify Settings?"},
  {"Serial Settings."},
  {"Use Serial?:    "},
  {"Baudrate:       "},
  {"Test entry:     "},
  {"Test entry:     "},
};

boolean answer = 1; //0 is no, and 1 is yes
#define no 0
#define yes 1

int BAUD;

IOlcd lcd;
PS2Keyboard keyboard;
const int DataPin = 3;
const int IRQpin = 2;

char Buffer[32];
int BufferIndex = 0;

void setup(){
  lcd.init(0x20, MCP23016);
  keyboard.begin(DataPin, IRQpin);
  emptyBuffer();
  lcd.begin(16,2);
  lcd.home();
  lcd.clear();
  askSettings();
}

void loop(){
  char action = KeyboardInput();
  delay(100);
}

void askSettings(){
  char action;
  do{
    action = KeyboardInput();
    lcd.setCursor(0,1);
    lcd.print(initialSettings[1]);
  }
  while(action != PS2_ESC);
}

char KeyboardInput(){
  if(keyboard.available()){
    char c = keyboard.read();
    if(isprint(c)){
      if(BufferIndex>32){
        BufferIndex=0;
        emptyBuffer();
      }
      else{
        Buffer[BufferIndex] = c;
        BufferIndex++;
      }
    }
    else{
      return c;
    }
  }
}

void printScreen(){
  for(int i=0;i<32;i++){
    if(i>=16&&Buffer[i]!='\0'){
      lcd.setCursor((i-16),1);
      lcd.print(Buffer[i]);
    }
    else if(Buffer[i]!='\0'){
      lcd.setCursor(i,0);
      lcd.print(Buffer[i]);
    }
  }
}

void emptyBuffer(){
  for(int i=0;i<32;i++){
    Buffer[i] = '\0';
  }
  lcd.clear();
}
