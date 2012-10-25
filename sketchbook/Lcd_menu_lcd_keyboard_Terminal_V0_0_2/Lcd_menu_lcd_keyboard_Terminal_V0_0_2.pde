#include <IOlcd.h>
#include <Wire.h>
#include <PS2Keyboard.h>

int MenuIndex=1;
char Menu[9] [17] = {
  {"Select Program: "},
  {"1.Serial Monitor"},
  {"2. Test entry   "},
};
char initialSettings[5] [17]={
  {"Modify Settings?"},
  {"baudrate:       "},
  {"Use Serial?:    "},
  {"Test entry:     "},
  {"Test entry: 2   "},
};

char settingsBuff[16];

int baud;

IOlcd lcd;
PS2Keyboard keyboard;
const int DataPin = 3;
const int IRQpin = 2;

char Buffer[32];
int BufferIndex = 0;

int ledpin = 7;
char keyword[7] = {"led on"};

void setup(){
  pinMode(ledpin, OUTPUT);
  lcd.init(0x20, MCP23016);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  emptyBuffer();
  lcd.begin(16,2);
  askSettings();
  lcd.home();
  lcd.clear();
}

void loop(){
  char action = KeyboardInput();
  delay(100);
  printScreen();
  stringcheckled();
}

void loadProgram(){
}

void askSettings(){
  char action;
  do{
    action = KeyboardInput();
    lcd.setCursor(0,0);
    lcd.print(initialSettings[0]);
    lcd.setCursor(0,1);
    lcd.print("yes/no enter/esc");
    if(action == PS2_ENTER){
      lcd.clear();
      do{
        action = KeyboardInput();
        switch(action){
          case PS2_DOWNARROW:
          MenuIndex++;
          if(MenuIndex==6){
            MenuIndex=1;
          }
          break;
          case PS2_UPARROW:
          MenuIndex--;
          if(MenuIndex<1){
            MenuIndex=5;
          }
          break;
          case PS2_ENTER:
          Serial.println(MenuIndex, DEC);
          if(MenuIndex==1){
            baud = atoi(Buffer);
            Serial.println("baud");
            Serial.println(baud, DEC);
            //proves its a real number;
            Serial.println(baud*2, DEC);
            emptyBuffer();
          }
          else if(MenuIndex==2){
            
          }
          else if(MenuIndex==3){
            Serial.println("third setting");
          }
          emptyBuffer();
          MenuIndex++;
          break;
        }
        printmenu();
      }
      while(action != PS2_ESC);
    }
  }
  while(action != PS2_ESC);
}

void printmenu(){
  lcd.setCursor(0,0);
  lcd.print(initialSettings[MenuIndex]);
  lcd.setCursor(0,1);
  lcd.print(Buffer);
}

char KeyboardInput(){
  if(keyboard.available()){
    char input = keyboard.read();
    if(isprint(input)){
      if(BufferIndex>32){
        BufferIndex=0;
        emptyBuffer();
      }
      else{
        Buffer[BufferIndex] = input;
        BufferIndex++;
      }
    }
    else{
      return input;
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

void stringcheckled(){
  if(!strcmp(keyword, Buffer)){
    digitalWrite(ledpin, HIGH);
  }
  else{
    digitalWrite(ledpin, LOW);
  }
}

void emptyBuffer(){
  for(int i=0;i<32;i++){
    Buffer[i] = '\0';
  }
  lcd.clear();
}
