#include <Wire.h>;
#include <IOlcd1_0.h>
#include <TrueRandom.h>

#define width 16
#define height 2
#define fieldSize width*height
#define byte_type int8_t
byte_type field[fieldSize+1];

IOlcd1_0 lcd;

int fieldIndex = 0;

void fillField(){
  for(int i=0;i<=fieldSize-1;i++){
    field[i]=TrueRandom.random(0,1);
    field[i+1]='\0';
  }
}
void displayFieldOnce(){
  for(int i=0;i<=fieldSize-1;i++){
    Serial.print(field[i]);
    if(i%width==0){
      Serial.println("");
    }
  }
}

void setup(){
  Serial.begin(115200);
  lcd.init(0x20, MCP23016);
  lcd.begin(width, height);
  lcd.clear();
  fillField();
}

void loop(){
  
}
