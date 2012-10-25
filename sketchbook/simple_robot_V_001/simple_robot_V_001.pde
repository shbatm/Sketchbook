#include <Servo.h>

Servo stearing;

int pos;
int sensorLDR = A0;
int E2 = 5;
int M2 = 4;
int speedpot = A1;
int value;


void setup()
{
  stearing.attach(9);
  pinMode(A0, INPUT);
  Serial.begin(9800);
  pinMode(M2, OUTPUT);
  pinMode(speedpot, INPUT);
}

void loop()
{
  value = analogRead(speedpot);
  value = map(value, 1023, 0, 255, 0);

  pos = analogRead(sensorLDR);
  pos = map(pos, 60, 400, 75, 90);
  
  

  if (pos >= 75 && pos <= 90){
    stearing.write(pos);
    delay(100);    
  } 
  
  else
  {
    stearing.write(90);
    delay(100);
  }
  
    {
      digitalWrite(M2, HIGH);
      analogWrite(E2, value);
    }

  Serial.println("");
  Serial.println(pos);
  Serial.println(analogRead(A0));
  Serial.println("");

  //delay(1000);
}

