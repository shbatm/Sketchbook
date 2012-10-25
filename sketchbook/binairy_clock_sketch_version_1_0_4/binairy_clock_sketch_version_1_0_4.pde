int latchpin = 8;
int clockpin = 12;
int datapin = 11;

int seconds = A4;
int mins = A0;
int hours = A2;

int button1 = 2;
int button2 = 3;
int button3 = 4;

int state3;
int state2;
int state1;
int state = 0;

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
  state1 = digitalRead(button1);
  state2 = digitalRead(button2);
  state3 = digitalRead(button3);
    
    if(state1 == 1){
      state = 1
      settime();
    }
    
    timerules();
    
}

void timerules(){
  
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

void settime(){  
    
    if (state == 1){
          
          if (state2 == 1){
            countminuts++;
            lcd.setCursor(0,0);
            lcd.print("minuts:");
            lcd.print(countminuts);
          }
          
            if (state3 == 1){
              counthours++;
              lcd.setCursor(0,1);
              lcd.print("hours:");
              lcd.print(counthours);
            }
        }
    if(state == 1 && state1 == 1){
       state = 0;
    }
}


void showtime(){
  
  for (int i = 0; i < 500; i++ ){
        
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
    //on the first line of the lcd we write Time:
    lcd.setCursor(0,0);
    lcd.print("Time:");
      
      //on the second line we write seconds:minuts:hours
      lcd.setCursor(0,1);
      lcd.print("  ");
      lcd.setCursor(0,1);
      lcd.print(countseconds);
      lcd.setCursor(2,1);
      lcd.print(":");
      lcd.print("  ");
      lcd.setCursor(3,1);
      lcd.print(countminuts);
      lcd.setCursor(5,1);
      lcd.print(":");
      lcd.print("  ");
      lcd.setCursor(6,1);
      lcd.print(counthours);
}
