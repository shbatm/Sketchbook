int latchpin = 8;
int clockpin = 12;
int datapin = 11;

int seconds = A4;
int mins = A0;
int hours = A2;

int button1 = 2;
int button2 = 3;

int state;
int buttonstate;

int countseconds;
int countminuts;
int counthours;

int dval;

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
 Serial.begin(9600);
}

//#define STATE_TIME_SETUP 0
#define STATE_TIME_SEC 0
#define STATE_TIME_MINS 1
#define STATE_TIME_HOURS 2
#define STATE_TIME_DISPLAY 3
#define BUTTON_UNPRESSED 0
#define BUTTON_PRESSED 1

void loop(){
  //Serial.println(digitalRead(button1));    
 dval = 100; 
      
      if(digitalRead(button1)){
        buttonstate = BUTTON_PRESSED;        
        
        if (state == STATE_TIME_SEC){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Seconds:");
            lcd.setCursor(0,1);
            lcd.print(countseconds);
            countseconds++;
            delay(dval);
            dval = 10;
            timerules();
          
        }
        
        if (state == STATE_TIME_MINS){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("minutes:");
        }
        if(state == STATE_TIME_HOURS){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("hours:");      
        }
        if(state == STATE_TIME_DISPLAY){
          timerules();
          dval = 500;
        }
        
        
      else {
        buttonstate = BUTTON_UNPRESSED;
      }
    
    
    
    } else if (buttonstate == BUTTON_PRESSED) {
      if(digitalRead(button1)){
        buttonstate = BUTTON_PRESSED;
        
      }else{
        buttonstate = BUTTON_UNPRESSED;
        state++;
        delay(1);
      }
    }
}


void timerules(){
  countseconds++;

  if (countseconds == 60){
    countminuts++;
    countseconds = 0;
    if (countminuts == 60){
      counthours++;
      countminuts = 0;  
      if (counthours == 24){
        counthours = 0;
      }
    }
  }
      
  showtime();
  //writelcd();
    
}

void showtime(){
  
  for (int i = 0; i < dval; i++ ){
        
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
  lcd.clear();  
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
