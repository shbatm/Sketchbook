int buttonpin = 12;
int ledpin = 13;
int buttonvalue;

void setup(){
  pinMode(buttonpin, INPUT);
  pinMode(ledpin, OUTPUT);
  
}

void loop(){
  buttonvalue = digitalRead(buttonpin);
  if (buttonvalue == 1){
    digitalWrite(ledpin, HIGH);
    
  }
}
