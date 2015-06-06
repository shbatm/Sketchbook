/* YourDuino.com Example Software Sketch
 16 character 2 line I2C Display
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include "Wire.h"  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup()
{
  Wire.pins(0, 2);
  // initialize library
  lcd.begin(16,2);
  // blink backlight three times
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();
  // set cursor to positon x=0, y=0
  lcd.setCursor(0,0);
  // print Hello!
  lcd.print("Hello!");
  // wait a second.
  delay(1000);
  // set cursor on second line
  lcd.setCursor(0,1);
  // print www.tkkrlab.nl
  lcd.print("www.tkkrlab.nl");
}


void loop()
{
}
