void setup(){
  
}

void loop(){
  unsigned int addr = 0; // Variable ranges between 0 to 65,536 (memory size)
  for (addr = 0; addr <= 0x3FFF; addr++){
          int var = (addr, BIN);
          digitalWrite(2, HIGH && (var & 0b0000000000000001));
          digitalWrite(3, HIGH && (var & 0b0000000000000010));
          digitalWrite(4, HIGH && (var & 0b0000000000000100));
          digitalWrite(5, HIGH && (var & 0b0000000000001000));
          digitalWrite(6, HIGH && (var & 0b0000000000010000));
          digitalWrite(7, HIGH && (var & 0b0000000000100000));
          digitalWrite(8, HIGH && (var & 0b0000000001000000));
          digitalWrite(9, HIGH && (var & 0b0000000010000000));
          digitalWrite(10, HIGH && (var & 0b0000000100000000));
          digitalWrite(11, HIGH && (var & 0b0000001000000000));
          digitalWrite(12, HIGH && (var & 0b0000010000000000));
          digitalWrite(13, HIGH && (var & 0b0000100000000000));
          digitalWrite(A1, HIGH && (var & 0b0001000000000000));
          digitalWrite(A2, HIGH && (var & 0b0010000000000000));
          digitalWrite(A3, HIGH && (var & 0b0100000000000000));
          digitalWrite(A4, HIGH && (var & 0b1000000000000000));
        
        
          delayMicroseconds(50);}
}
