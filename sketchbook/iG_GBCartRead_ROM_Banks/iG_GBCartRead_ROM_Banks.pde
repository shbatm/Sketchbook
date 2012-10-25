/*
 GBCartRead ROM script - All ROM banks
 Version: 1.0
 Author: Alex from Inside Gadgets (http://www.insidegadgets.com)
 Created: 18/03/2011
 Last Modified: 18/03/2011
 
 Read all ROM banks from a Gameboy Cartridge. Change the variable romBanks
 to the number of banks in your cartridge; this is found by reading the 
 Cartridge Header in ROM Bank 0
 
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
  
  // Reads D0 - D7, set as inputs 
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
  Serial.println("START"); // Send the start command
  
  int romBanks = 8; // Change to the number of ROM banks
  
  // Read x number of banks
    for (int y = 1; y < romBanks; y++) {
      selectROMbank(y);
      if (y > 1) {
        addr = 0x4000;
      }
      
      // Reads addresses 0000h to 7FFF in the first run, then
      // only 4000h to 7FFF in the second or more runs
      for (; addr <= 0x7FFF; addr++) {  
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, (addr >> 8));
        shiftOut(dataPin, clockPin, MSBFIRST, (addr & 0xFF));
        digitalWrite(latchPin, HIGH);
        delayMicroseconds(50);
        
        byte bval = 0;
        for (int z = 9; z >= 2; z--) {
          if (digitalRead(z) == HIGH) {
            bitWrite(bval, (z-2), HIGH);
          }
        }
        Serial.println(bval, DEC);
      }
  }
  Serial.println("END");

  while(1);
}

// Select the ROM bank by writing the bank number on the D0-D7 pins
void selectROMbank(int bank) {
  digitalWrite(rdPin, HIGH); // RD 1
  digitalWrite(wrPin, LOW); // WR 0
  delay(5);
  
  // Set D0-D7 as outputs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  unsigned int x = 0x2100; // Address where we write to
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (x >> 8));
  shiftOut(dataPin, clockPin, MSBFIRST, (x & 0xFF));
  digitalWrite(latchPin, HIGH);
  delay(5);
  
  // Select the bank
  if (bank & 1) { 
    digitalWrite(2, HIGH);
  }
  if (bank & 2) { 
    digitalWrite(3, HIGH);
  }
  if (bank & 4) { 
    digitalWrite(4, HIGH);
  }
  if (bank & 8) { 
    digitalWrite(5, HIGH);
  }
  if (bank & 16) { 
    digitalWrite(6, HIGH);
  }
  if (bank & 32) { 
    digitalWrite(7, HIGH);
  }
  if (bank & 64) { 
    digitalWrite(8, HIGH);
  }
  if (bank & 128) { 
    digitalWrite(9, HIGH);
  }
  delay(5);
  
  // Set back to reading ROM
  digitalWrite(rdPin, LOW); // RD 0
  digitalWrite(wrPin, HIGH); // WR 1
  
  // Reset outputs to LOW
  for (int l = 2; l <= 9; l++) {
    digitalWrite(l, LOW);
  }
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  delay(5);
}

