int sensorLDR = A0;
int ledpin = 13;

#include <Servo.h>

void setup()
{
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9800);
}

void loop()
{
  int SensorLDR = analogRead(sensorLDR);
  
  if (SensorLDR >= 490)
    {
      digitalWrite(ledpin, HIGH);
    }
  
  else 
    {
      digitalWrite(ledpin, LOW);
    }
    
  Serial.println(analogRead(sensorLDR));
  delay(100);
  
}

  
