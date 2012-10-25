int pota;
int potb;
int potc;
//int buttonstate;
int apin = A0;
int bpin = A1;
int cpin = A2;
//int buttonpin = 10;
int ledpin = 13;

//include a library that works with my lcd.
#include <LiquidCrystal440.h>
//set pins to use for lcd screen connections
LiquidCrystal lcd(12, 11, 2, 3 , 4, 13, 5, 6, 7, 8);

void setup(){
  //set pinmodes.
  pinMode(apin, INPUT);
  pinMode(bpin, INPUT);
  pinMode(cpin, INPUT);
  pinMode(9, OUTPUT);
  //pinMode(buttonpin, INPUT);
  pinMode(ledpin, OUTPUT);
  //set screen brightness.
  analogWrite(9, 70);
  lcd.begin(8,2);
    
  //print on the first halve the bA0, (bit value, pin A0)
  lcd.setCursor(0,0);
  lcd.print("bA0:");
  //print on the second halve the bA1, (bit value, pin A1)
  lcd.setCursor(0,1);
  lcd.print("bA1:");
  
  //start serial to debug.
  Serial.begin(9600);
  delay(1000);
}

  void loop(){
    //clear lcd just in case.
    //lcd.clear();
    
    //read the value's from the potmeters
    pota = (analogRead(apin));
    potb = (analogRead(bpin));
    potc = (analogRead(cpin));
    
    //read the button state.
    //buttonstate = digitalRead(buttonpin);
    //see over serial what the button state is.
    //Serial.println(buttonstate);
    //test to see if the buton works.
    //if(buttonstate == 1){
      
      digitalWrite(ledpin, HIGH);
     
      //print the data next to bA0      
      lcd.setCursor(4,0);
      lcd.print(pota);
      
      //print the data next to bA1
      lcd.setCursor(4,1);
      lcd.print(potb);
      
    //}
    //else{
      digitalWrite(ledpin, LOW);
      delay(100);
    //}
  }
