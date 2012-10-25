int buttonpin = 13;
int buttonvalue;

void setup(){
  pinMode(buttonpin, INPUT);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello.");
  delay(1000);
}

void loop(){
  buttonvalue = digitalRead(buttonpin);
  Serial.println(buttonvalue);
  delay(1000);
  
  if (buttonvalue == 1){
    digitalWrite(A5, HIGH);
  }
}
