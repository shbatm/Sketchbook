int latchpin = 8;
int clockpin = 12;
int datapin = 11;

int seconds = A4;
int mins = A0;
int hours = A2;

int countseconds;
int countminuts;
int counthours;

int x;
int speedpin = A1;

#include <LiquidCrystal.h>
LiquidCrystal lcd (13, 10, 9, 7, 6, 5);

void setup(){
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
  pinMode(seconds, OUTPUT);
  pinMode(mins, OUTPUT);
  pinMode(hours, OUTPUT);
  
  lcd.begin(16,2);
  lcd.print("hello, world");
  delay(1000);
  lcd.clear();
  
}

void loop(){
  
  for(countseconds = 0; countseconds < 60; countseconds++ ){
    
    if (countseconds == 59){
      countminuts++;
    }
      
      if (countminuts == 59){
        counthours++;
        countminuts = 0;
      }
      
        if (counthours == 23 && countminuts == 59){
          counthours = 0;
        }
        
          showtime();
          writelcd();
  }
}

void showtime(){
  x = analogRead(speedpin);
  x = map(x, 1, 1023, 1, 1000);
  
  for (int i = 0; i < x; i++ ){
        
    digitalWrite(latchpin, LOW);
    digitalWrite(seconds, LOW);
    shiftOut(datapin, clockpin, MSBFIRST, countseconds);    
    digitalWrite(seconds, HIGH);
    digitalWrite(latchpin, HIGH);
    delayMicroseconds(250);     
    
      digitalWrite(latchpin, LOW);
      digitalWrite(mins, LOW);
      shiftOut(datapin, clockpin, MSBFIRST, countminuts);
      digitalWrite(mins, HIGH);
      digitalWrite(latchpin, HIGH);
      delayMicroseconds(250);    
      
        digitalWrite(latchpin, LOW);
        digitalWrite(hours, LOW);
        shiftOut(datapin, clockpin, MSBFIRST, counthours);
        digitalWrite(hours, HIGH);
        digitalWrite(latchpin, HIGH);
        delayMicroseconds(500);
  }
}

void writelcd(){
    lcd.setCursor(0,0);
    lcd.print("Time:");
    
      lcd.setCursor(0,1);
      lcd.print("00");
      lcd.setCursor(0,1);
      lcd.print(countseconds);
      lcd.setCursor(2,1);
      lcd.print(":");
      lcd.print("00");
      lcd.setCursor(3,1);
      lcd.print(countminuts);
      lcd.setCursor(5,1);
      lcd.print(":");
      lcd.print("00");
      lcd.setCursor(6,1);
      lcd.print(counthours);
}
