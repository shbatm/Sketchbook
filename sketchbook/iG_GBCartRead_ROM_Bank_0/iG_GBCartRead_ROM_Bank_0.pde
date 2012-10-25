/*
 GBCartRead ROM script - Bank 0
 Version: 1.0
 Author: Alex from Inside Gadgets (http://www.insidegadgets.com)
 Created: 18/03/2011
 Last Modified: 18/03/2011
 
 Read only Bank 0 from a Gameboy Cartridge.
 
 */

int latchPin = 10;
int dataPin = 11;
int clockPin = 12;
int rdPin = A5;
int wrPin = 13;

void setup() {
  Serial.begin(57600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(rdPin, OUTPUT);
  pinMode(wrPin, OUTPUT);
  
  // Sets D0 - D7, set as inputs 
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
}

void loop() {
  
  unsigned int addr = 0; // Variable ranges between 0 to 65,536
  
  digitalWrite(rdPin, LOW); // RD 0
  digitalWrite(wrPin, HIGH); // WR 1
      
  Serial.println("START"); // Send the start command
  
  // Read addresses 0000h to 3FFF, i.e. ROM bank 0
  for (addr = 0; addr <= 0x3FFF; addr++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (addr >> 8)); // Most left 8 bits transferred
    shiftOut(dataPin, clockPin, MSBFIRST, (addr & 0xFF)); // Most right 8 bits transferred
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(50); // So ROM can process our request
    
    byte bval = 0;
    for (int z = 9; z >= 2; z--) {
      if (digitalRead(z) == HIGH) {
        bitWrite(bval, (z-2), HIGH);
      }
    }
    Serial.println(bval, DEC);
  }
  Serial.println("END");

  while(1);
}
