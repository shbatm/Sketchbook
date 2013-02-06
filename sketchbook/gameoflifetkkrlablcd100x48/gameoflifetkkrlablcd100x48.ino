#include <TrueRandom.h>
#include <SED1531.h>
SED1531 lcd;


#define screenWidth  100
#define screenHeigth  48
#define fieldSize screenWidth*screenHeigth+1
byte screenBuff[fieldSize];

void fillField(byte *buffer){
  for(int i = 0;i<fieldSize;i++){
    buffer[i] = TrueRandom.random(0,2);
  }
}

void Display(byte *buffer){
  for(int i = 0;i<fieldSize;i++){
    if(!(i%20)){
      Serial.println("");
    }
    Serial.print(buffer[i]);
    lcd.print(buffer[i]);
  }
}

void setup(){
  lcd.begin();
  lcd.setCursor(0);
  lcd.setContrast(32);
  delay(2000);
  Serial.begin(115200);
  //lcd.inverse(true);
  fillField(screenBuff);
  Display(screenBuff);
}

void loop(){}
