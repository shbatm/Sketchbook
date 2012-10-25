//the necesarry librarys needed to make this work. (because of the i/o expander)
#include <IOexpander.h>
#include <Wire.h>;
#include <inttypes.h>;

//make a IOexpander attachement.
IOexpander IOexp;

//define the pins we are gonna use first read and write pins.
int rdpin = 12;
int wrpin = 13;

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
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    
  }
  
  void loop(){
    //a variable for counting from 0 to 65,536 unsigned because then it won't go negative.
    unsigned int addr = 0;
    //set read write pins so we can read from the rom.
    digitalWrite(rdpin, LOW);
    digitalWrite(wrpin, HIGH);
    //send start comand so the python script know's when to read. credits to Alex from inside gadgets (borowed kinda)
    Serial.println("START");
    
      //basicly read adress from 0000h to 3FFFF
      for (addr = 0; addr <= 0x3FFF; addr++){
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
          
          
          {
          int D1;
          int D2;
          int D3;
          int D4;
          int D5;
          int D6;
          int D7;
          int D8;
          int Data;
          
          D1 = digitalRead(2);
          D2 = digitalRead(3);
          D3 = digitalRead(4);
          D4 = digitalRead(5);
          D5 = digitalRead(6);
          D6 = digitalRead(7);
          D7 = digitalRead(8);
          D8 = digitalRead(9);
          
         
          //Serial.println(Data);
           
           Serial.print(D1);
           Serial.print(D2);
           Serial.print(D3);
           Serial.print(D4);
           Serial.print(D5);
           Serial.print(D6);
           Serial.print(D7);
           Serial.print(D8);
           Serial.println(" ");
          }
          
      }
      Serial.println("END");
      while(1);
    
  }
