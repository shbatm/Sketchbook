#include <Wire.h> // specify use of Wire.h library.



#define SW0  4
#define SW1  5
#define ledPin  13


// names can't have number as first char.
const byte HOME = 0x02;
const byte CL = 0x10; // cursor left
const byte CR = 0x14; // cursor right
const byte SL = 0x18; // Shifts entire display left
const byte SR = 0x1c; // Shifts entire display right 


int j;

void setup()

{
  pinMode(SW0, INPUT);  // for this use a slide switch
  pinMode(SW1, INPUT);  // N.O. push button switch
  pinMode(ledPin, OUTPUT);
  digitalWrite(SW0, HIGH); // pull-ups on
  digitalWrite(SW1, HIGH);

  Wire.begin();
  Wire.beginTransmission(0x20);  // set mcp23016 output
  Wire.send(0x06); 
  Wire.send(0x00);  // DDR Port0 all output
  Wire.send(0x0F);  // DDR 0-3 input 4-7 output
  Wire.endTransmission(); 
  // setup port 1 D7 = E; D6 = RS 
  Wire.beginTransmission(0x20); 
  Wire.send(0x01); //pointer
  Wire.send(B10000000); // setup for command mode
  Wire.endTransmission(); 


  writeCommand(0x38); // 2 lines
  writeCommand(0x0F); // blinking cursor
  CLR(); // clear dispaly
  writeCommand(HOME);



}



void loop()   {


  CLR(); // clear display

  typeln("Hello Lewis!\n", 1); // write to display line 1
  typeln("You got it! \n", 2); // write to display line 2
  toggle(ledPin);
  delay(500);
} // end loop


// Below we pass a pointer to array1[0]. If no '\n' then the limit is 16 char. (0 - 15)
void typeln(char *array1, int i)   {
  delayMicroseconds(1000);
  if (i == 1) writeCommand(0x80); // begin on 1st line
  if (i == 2) writeCommand(0x80 + 0x40); // begin on 2nd line 
  for (int j = 0; (array1[j] != '\n') && (j < 16); j++) type(array1[j]); 
}




// send command to HD44780 display 
//  E High to Low transition write command or data to HD44780 display
//  RS 0 for command, default 1 for data
//  setup port1 D7 = E; D6 = RS 
void writeCommand(byte x)   {

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x00); // begin here
  Wire.send(x);  // command code
  Wire.endTransmission();

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x01); // pointer
  Wire.send(B10000000);  // command mode E high
  Wire.endTransmission();

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x01); // pointer 
  Wire.send(B00000000);  // char mode E low
  Wire.endTransmission();

  delayMicroseconds(100);

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x01); //  pointer
  Wire.send(B10000000);  // E high back to command mode
  Wire.endTransmission();


} 

//  send char to HD44780 display 
//  E High to Low transition write command or data to HD44780 display
//  RS 0 for command, default 1 for data
//  setup port1 D7 = E; D6 = RS 

void type(byte x)   {

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x00); // begin here
  Wire.send(x);  // data
  Wire.endTransmission();

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x01); // pointer
  Wire.send(B11000000);  // char mode E high
  Wire.endTransmission();

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x01); // pointer 
  Wire.send(B01000000);  // char mode E low
  Wire.endTransmission();

  delayMicroseconds(100);

  Wire.beginTransmission(0x20);  //  mcp23016 
  Wire.send(0x01); //  pointer
  Wire.send(B10000000);  // E high back to command mode
  Wire.endTransmission();


} 


// clear display
void  CLR()   {
  writeCommand(0x01); // clear
}


// toggle the state on a pin
void toggle(int pinNum) 
{  
  int pinState = digitalRead(pinNum);
  pinState = !pinState;
  digitalWrite(pinNum, pinState); 
}

