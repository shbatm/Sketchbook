#include <PS2Keyboard.h>
PS2Keyboard keyboard;
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,8,7,6,5);

const int DataPin = 4;
const int IRQpin = 3;

char text[32] = {0,0};
int Char = 0;
int y = 0;
int x = 0;
int z = 0;
int Switch = 0;
int Size = 32;
char key;

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
  if (keyboard.available()){
    psinterface();
    lcdinterface();
  }
}

void lcdinterface(){
  if(x==16){
    if(y!=1){
      y=1;
    }
    else{
      y=0;
    }
  }
  if(z==16){
    x=0;
  }
  else if(z==Size){
    lcd.clear();
    z=0;
    x=0;
  }
  lcd.print(text[z]);
  x++;
  z++;
  lcd.setCursor(x,y);
  lcd.cursor();
}

void psinterface(){
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
    lcd.clear();
    lcd.setCursor(0,0);
  }
  else if(key != PS2_ENTER){
    text[Char] = key;
    Char++;
    printbuffer();
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
