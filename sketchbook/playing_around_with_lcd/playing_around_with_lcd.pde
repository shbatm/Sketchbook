int pota;
int potb;
int potc;
int apin = A0;
int bpin = A1;
int cpin = A2;

//include a library that works with my lcd.
#include <LiquidCrystal.h>
//set pins to use for lcd screen connections
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
  //set pinmodes.
  pinMode(apin, INPUT);
  pinMode(bpin, INPUT);
  pinMode(cpin, INPUT);
  pinMode(6, OUTPUT);
  //set screen brightness.
  analogWrite(6, 70);
  lcd.begin(8,2);
  delay(1000);
}

  void loop(){
    int dval = (potc);

    //read the value's from the potmeters
    pota = (analogRead(apin));
    potb = (analogRead(bpin));
    potc = (analogRead(cpin));
    // potc = map(potc, 0, 16, 0, 1023);
    
    //clear lcd just in case.
    lcd.clear();
    //set cursor at the start, just in case.
    lcd.setCursor(0,0);
    
    //print on the first halve the data we get from pin A0.
    lcd.print("bA0:");
    lcd.print(pota);
    //delay(dval);
    
    //print on the second halve the data we get from pin A1.
    lcd.setCursor(0,1);
    lcd.print("bA1:");
    lcd.print(potb);
    
    delay(dval-(dval/2));
  }
