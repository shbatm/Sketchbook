int potvalue = A0;

void setup(){
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  
}

void loop(){
  int delayvalue;
  delayvalue = analogRead(potvalue);
  
  
  {
    
    for(int addr = 6; addr <= 13; addr++){
    digitalWrite(addr, HIGH);
    delay(delayvalue);
    digitalWrite(addr, LOW);
     
    
      }
    
    for(int addr = 12;addr >= 6; addr--){
        digitalWrite(addr, HIGH);
        delay(delayvalue);
        digitalWrite(addr, LOW);
      }
    
    
  
  
 }  
  
  
  
}
