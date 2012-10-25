/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 25 July 2009
 by David A. Mellis
 
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 11, 5, 4, 3, 2);

int contr = 9;
int potpin = A5;
int ledpin = 13;
int contrval;
int button = 7;
int buttonval;
int button2 = 10;
int buttonval2;

void setup() {
  pinMode(contr, OUTPUT);
  pinMode(potpin, INPUT);
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, HIGH);
  // set up the LCD's number of rows and columns: 
  lcd.begin(8, 2);
  Serial.begin(9600);
}

void loop() {
  //a try to adjust contrast digitaly.
//  contrval = analogRead(potpin);
//  contrval = map(contrval, 0, 1023, 0, 255);
  
  analogWrite(contr, contrval);
  
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);
  lcd.print("Hello World");
  delay(1000);
  //looking in what state the second button is.
  buttonval2 = digitalRead(button2);
  //looking what the state of the button is.   
  buttonval = digitalRead(button);
  
  if(buttonval > 0){
    //set cursor in begining of screen.
    lcd.setCursor(0, 0);
    //set the display to auto scroll.
    lcd.autoscroll();
    //print the numbers 0 till 9.
    for (int thisChar = 0; thisChar < 10; thisChar++){
      lcd.print(thisChar);
      delay(500);
    }
  }  
  
  if (buttonval2 > 0){
    lcd.clear();
    delay(200);
    lcd.setCursor(0, 0);
    lcd.print("hoi ben Robert");
    delay(1000);
    lcd.clear();
  }
  
//  else if(buttonval < 1){     
//     lcd.clear();
//     delay(1000);
//    }
    
  else {
    lcd.noAutoscroll();
    lcd.clear();
    delay(1000);
  }
  
  
  
//  Serial.println("pot value:");
//  Serial.println(analogRead(potpin));
//  Serial.println("contrast value:");
//  Serial.println(contrval);
//  Serial.println("contr variable");
//  Serial.println(contr);
//  Serial.println(" ");
//  Serial.println(buttonval);
  
}

