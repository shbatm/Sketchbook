int potval;
int potpin = A0;
int potval1;
int potpin1 = A1;

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(potpin, INPUT);
  pinMode(potpin1, INPUT);
}

void loop() {
  potval = analogRead(potpin);
  potval1 = analogRead(potpin1);
  
 lcd.setCursor(0,0);
 lcd.print("A0b:");
 lcd.setCursor(4,0);
 lcd.print(potval);
 lcd.print("    ");
 
 lcd.setCursor(8,0);
 lcd.print("A1b:");
 lcd.setCursor(12,0);
 lcd.print(potval1);
 lcd.print("    ");
 
}

