#include <PS2Keyboard.h>
PS2Keyboard keyboard;

const int DataPin = 4;
const int IRQpin = 3;

char text[32] = {0,0};
int Char = 0;
int Size = 32;
char key;

void setup(){
  keyboard.begin(DataPin, IRQpin); 
  Serial.begin(9600);
  for(Char=0;Char<=Size;Char++){
    text[Char] = 32;
  }
  Char = 0;
  Serial.println("Ready to type.");
}

void loop(){
  if (keyboard.available()){
    key = keyboard.read();
    if(Char == 32){
      flushbuffer();
      Char = 0;
    }
    if(key == PS2_BACKSPACE){
      text[Char] = 32;
      Char--;
      printbuffer();
    }
    if((key != PS2_BACKSPACE) || (key != PS2_ENTER)){
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
