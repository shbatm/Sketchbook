/*
  LiquidCrystal Library - Cursor
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD and
 uses the cursor()  and noCursor() methods to turn
 on and off the cursor.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 8, 7, 6, 5);

#include <PS2Keyboard.h>

const int DataPin = 4;
const int IRQpin = 3;
int x;
int y;

PS2Keyboard keyboard;

void setup() {
 delay(1000);
 keyboard.begin(DataPin, IRQpin);
 lcd.begin(16,2);
 Serial.begin(9600);
 Serial.println("keyboard available?");
}

void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    
    if(c == PS2_ENTER){
      y = 1;
      x = 0;
    }
    if(c == PS2_ENTER && y == 1){
      y = 0;
    }
    
    if(c == PS2_BACKSPACE && x > 0){
      x--;
      lcd.setCursor(x,y);
      lcd.print(" ");
            
    }
    else{
      lcd.print(c);
      x++;
    }
  }
  lcd.setCursor(x,y);
}

