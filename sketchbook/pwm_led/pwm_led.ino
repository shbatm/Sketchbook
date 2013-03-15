int ledpin1 = 13;
int ledpin2 = 12;

void setup(){
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  Serial.begin(115200);
}

int i = 1;
int Step = 1;
int delayVal = 1000;
int factor = 1;
unsigned long previousMillis = 0;

void loop(){  
  analogWrite(ledpin1,i);
  analogWrite(ledpin2,i);
  if(Serial.available()){
    char inbyte = Serial.read();
    switch(inbyte){
      case '+':
        delayVal += pow(10,factor);
        break;
      case '-':
        delayVal -= pow(10,factor);
      default:
        if(isdigit(inbyte)){
          factor = inbyte-48;
        }
    }
  }
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > delayVal){
    previousMillis = currentMillis;
    if(i>254 || i < 0){
      Step*=-1;
    }
    i+=Step;
    Serial.println(i);
    Serial.print("delayValue:");
    Serial.println(delayVal);
    Serial.print("factor: ");
    Serial.println(factor);
  }
}
