#include <SED1531.h>

SED1531 lcd;

byte image[] = {
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101
};

byte heart[] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte smiley[] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

void setup(){
  Serial.begin(115200);
  lcd.begin();
  lcd.setCursor(0);
  lcd.setContrast(16);
  lcd.println("bam");
  lcd.println("bam");
  lcd.println("bam");
  lcd.println("bam");
  lcd.println("bam");
  lcd.println("bam");
  lcd.println("worked!");
  lcd.println("Hello World!");
  lcd.print("this is a long line yea it is long line");
  delay(2000);
  lcd.inverse(true);
//  lcd.write(1);
//  lcd.write(2);
//  lcd.write(3);
//  lcd.write(0);
//  for(int i = 0;i<5;i++){
//    lcd.write(image[i]);
//  }
//  lcd.write(0);
//  for(int i = 0;i<8;i++){
//    lcd.write(heart[i]);
//  }
//  lcd.write(0);
//  for(int i=0;i<8;i++){
//    lcd.write(smiley[i]);
//  }
}
boolean state = 1;
void loop(){
  for(int c = 0;c<31;c++){
    lcd.setContrast(c);
    delay(20);  
  }
  for(int c=31;c>0;c--){
    lcd.setContrast(c);
    delay(15);
  }
  lcd.inverse(state = !state);
}
