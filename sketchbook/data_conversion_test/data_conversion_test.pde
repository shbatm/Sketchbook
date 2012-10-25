

void setup() {
  // put your setup code here, to run once:

  
  Serial.begin(9600);
  Serial.println('5FFF', BIN);
  delay(1000);
  Serial.println(100011001000110, HEX);

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
