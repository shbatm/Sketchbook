#include <PS2Keyboard.h>

const int DataPin = 8;
const int IRQpin = 2;

PS2Keyboard keyboard;

int ledpin = 13;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
    
    pinMode(ledpin, OUTPUT);
}

void loop(){
  if (keyboard.available()){
    char c = keyboard.read();
    
    if (c == PS2_ENTER){
      digitalWrite(ledpin, HIGH);
      
    }
    
    else {
      digitalWrite(ledpin, LOW);
    }
  }
}
