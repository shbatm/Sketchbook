#include <IOexpander.h>
#include <Wire.h>;
#include <inttypes.h>;

IOexpander IOexp;

int rdpin = 12;
int wrpin = 13;

void setup(){
  //set read and write as output
  pinMode(rdpin, OUTPUT);
  pinMode(wrpin, OUTPUT);
  
  //set datalines as input.
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  //start serial comunication.
  Serial.begin(57600);
  //look if the i/o expander works.
    if(IOexp.init(0x20, MCP23016))
  Serial.println("Communication with IOexpander works!");
    else
  Serial.println("No communication with the IOexpander!!");
  
 //pins on Adreslines as output.
 IOexp.pinModePort(0,OUTPUT);
 IOexp.pinModePort(1,OUTPUT);
}

void loop(){
  unsigned int addr = 0; //variable ranges between 0 to 65,536
  
 digitalWrite(rdpin, LOW); // RD 0
 digitalWrite(wrpin, HIGH); // WR 1
 
 Serial.println("START"); // Send the start command
  
  //Read Addresses 0000hh to 3fff, i.e. Rom bank 0
  for (addr = 0; addr <= 0x3FFF; addr++){
    int value = addr;
    IOexp.digitalWrite(0,0, HIGH && (value & 0x0001));
    IOexp.digitalWrite(0,1, HIGH && (value & 0x0002));
    IOexp.digitalWrite(0,2, HIGH && (value & 0x0004));
    IOexp.digitalWrite(0,3, HIGH && (value & 0x0008));
    IOexp.digitalWrite(0,4, HIGH && (value & 0x0010));
    IOexp.digitalWrite(0,5, HIGH && (value & 0x0020));
    IOexp.digitalWrite(0,6, HIGH && (value & 0x0040));
    IOexp.digitalWrite(0,7, HIGH && (value & 0x0080));
    IOexp.digitalWrite(1,0, HIGH && (value & 0x0100));
    IOexp.digitalWrite(1,1, HIGH && (value & 0x0200));
    IOexp.digitalWrite(1,2, HIGH && (value & 0x0400));
    IOexp.digitalWrite(1,3, HIGH && (value & 0x0800));
    IOexp.digitalWrite(1,4, HIGH && (value & 0x1000));
    IOexp.digitalWrite(1,5, HIGH && (value & 0x2000));
    IOexp.digitalWrite(1,6, HIGH && (value & 0x4000));
    IOexp.digitalWrite(1,7, HIGH && (value & 0x8000));
    
  delayMicroseconds(50);//give some time to process our request
  
  byte bval = 0;
  for (int z = 9; z <= 2; z--){
    if (digitalRead(z) == HIGH){
      bitWrite(bval, (z-2), HIGH);
    }
  }
  Serial.println(bval, DEC);
  }
    
    Serial.println("END");
    
    while(1);
  
}
