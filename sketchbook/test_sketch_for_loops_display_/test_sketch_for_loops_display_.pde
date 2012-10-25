int aapin = 0;
int potvalue;
int potvalue2;
int potvalue3;
int bbpin = 1;
int conpin = 6;
int ccpin = 2;

#include <LiquidCrystal440.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup (){
  pinMode(aapin, INPUT);
  pinMode(bbpin, INPUT);
  pinMode(ccpin, INPUT);
  pinMode(conpin, OUTPUT);
  analogWrite(conpin, 70);  
  lcd.begin(8,2);
  delay(100);
}
  
  void loop(){
    int dval = 101;
    lcd.clear();
    lcd.setCursor(0, 0);
    potvalue = (analogRead(aapin));
    potvalue2 = (analogRead(bbpin));
    potvalue3 = (analogRead(ccpin));
       
   //for(int i = 4; i < 8; i++){
   
     //if (i <= 8){
     lcd.print("bA0:");
     //lcd.setCursor(4, 0);
     lcd.print(potvalue);
     
     lcd.setCursor(0, 1);
     lcd.print("bA1:");
     lcd.print(potvalue2);
   //}
   
//     if (i >= 9){
//     lcd.setCursor(i, 1);
//     lcd.write(potvalue);
//   }
   delay(dval);
  //}
}
