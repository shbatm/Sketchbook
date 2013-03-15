#include <SED1531.h>
SED1531 lcd;
 
void setup(){
  Serial.begin(115200);
  lcd.begin();
  delay(200);
  lcd.inverse(true);
}

int val = 50;

void loop(){
  lcd.setCursor(0);
  lcd.print("bam");
  delay(val);
  //lcd.setCursor(1);
  lcd.print(" this is the second");
  delay(val);
  lcd.setCursor(2);
  lcd.print("this is the third");
  delay(val);
  lcd.setCursor(3);
  lcd.print("this is the fourth");
  delay(val);
  lcd.setCursor(4);
  lcd.print("12345678901234567890");
  delay(val);
}
