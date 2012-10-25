//goin to share conections over two arduino.

int wrPin = 10;
int rdPin = 11;

void setup(){
  pinMode(wrPin, OUTPUT);
  pinMode(rdPin, INPUT);
  
  //datapins are input
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  
  //Serial for cumunication.
  Serial.begin(57600);
  
}

  void loop(){
    unsigned int addr = 0;
    
    digitalWrite(rdPin, LOW);
    digitalWrite(wrPin, HIGH);
    
    Serial.println("START");
 for (addr = 0; addr <= 0x3FFF; addr++) {
        
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

