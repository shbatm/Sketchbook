//the necesarry librarys needed to make this work. (because of the i/o expander)
#include <IOexpander.h>
#include <Wire.h>;
#include <inttypes.h>;

//make a IOexpander attachement.
IOexpander IOexp;

//define the pins we are gonna use first read and write pins.
int rdpin = 3;
int wrpin = 2;

  void setup(){
    //start serial.
    Serial.begin(57600);
    //look if there is comunicatino with IOexpander.
    if(IOexp.init(0x20, MCP23016))
        Serial.println("Communication with IOexpander works!");
    else
        Serial.println("No communication with the IOexpander!!");
        
    //setting read write pins as output.
    pinMode(rdpin, OUTPUT);
    pinMode(wrpin, OUTPUT);
    //setting the pins on the IOexpander to output
    IOexp.pinModePort(0, OUTPUT);
    IOexp.pinModePort(1, OUTPUT);
    //setting data lines as input, so we can read them if we want.
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    
  }
  
  void loop(){
    //a variable for counting from 0 to 65,536 unsigned because then it won't go negative.
    unsigned int addr = 0;
    //set read write pins so we can read from the rom.
    //digitalWrite(rdpin, LOW);
    //digitalWrite(wrpin, HIGH);
    //send start comand so the python script know's when to read. credits to Alex from inside gadgets (borowed kinda)
    Serial.println("START");
    int romBanks = 8; // change to the number of rom banks
    
      // Read x number of banks
       for (int y = 1; y < romBanks; y++) {
       selectROMbank(y);
       if (y > 1) {
       addr = 0x4000;
       }
    
    
      // Reads addresses 0000h to 7FFF in the first run, then
       // only 4000h to 7FFF in the second or more runs
       for (; addr <= 0x7FFF; addr++) {
        int var = addr;
          IOexp.digitalWrite(0,0, HIGH && (var & 0x0001));
          IOexp.digitalWrite(0,1, HIGH && (var & 0x0002));
          IOexp.digitalWrite(0,2, HIGH && (var & 0x0004));
          IOexp.digitalWrite(0,3, HIGH && (var & 0x0008));
          IOexp.digitalWrite(0,4, HIGH && (var & 0x0010));
          IOexp.digitalWrite(0,5, HIGH && (var & 0x0020));
          IOexp.digitalWrite(0,6, HIGH && (var & 0x0040));
          IOexp.digitalWrite(0,7, HIGH && (var & 0x0080));
          IOexp.digitalWrite(1,0, HIGH && (var & 0x0100));
          IOexp.digitalWrite(1,1, HIGH && (var & 0x0200));
          IOexp.digitalWrite(1,2, HIGH && (var & 0x0400));
          IOexp.digitalWrite(1,3, HIGH && (var & 0x0800));
          IOexp.digitalWrite(1,4, HIGH && (var & 0x1000));
          IOexp.digitalWrite(1,5, HIGH && (var & 0x2000));
          IOexp.digitalWrite(1,6, HIGH && (var & 0x4000));
          IOexp.digitalWrite(1,7, HIGH && (var & 0x8000));
          
          delayMicroseconds(50);
          
          
          
            // the first pin in this list will be the topmost bit in the result (i.e. if it's the only bit that's HIGH, the result will be 128)
            const int pinsToRead[] = { 13, 12, 11, 10, 9, 8, 7, 6 };    
            const unsigned numberOfPins = sizeof( pinsToRead ) / sizeof( pinsToRead[ 0 ] );
            unsigned result = 0;
            for ( unsigned index = 0; index != numberOfPins; ++index )
            result = ( result << 1 ) + ( ( digitalRead( pinsToRead[ index ] ) == HIGH ) ? 1 : 0 );
            // writes number, in decimal, to serial port then newline.
            Serial.println( result, DEC );
          
          
      }
      Serial.println("END");
      while(1);
    
  }
