//Arduino PWM Speed Control：
//int E1 = 6;   
//int M1 = 7;
int E2 = 5;
int M2 = 4;

int speedpot = A1;
int value;

void setup() 
{ 
    //pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);   
    pinMode(speedpot, INPUT);
} 

void loop() 
{ 
  value = analogRead(speedpot); 
  value = map(value, 1023, 0, 255, 0);
  { 
    //digitalWrite(M1,HIGH);          
    //analogWrite(E1, value);   //PWM Speed Control
    digitalWrite(M2, HIGH);
    analogWrite(E2, value);
    delay(30); 
  }  
}


