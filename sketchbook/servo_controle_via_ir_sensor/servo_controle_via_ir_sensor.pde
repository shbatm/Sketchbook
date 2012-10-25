#include <Servo.h>

Servo stearing;

int pos = 0;
int sensorLDR = A0;

void setup()
{
  stearing.attach(9);
  pinMode(A0, INPUT);
  Serial.begin(9800);
}

void loop()
{
  pos = analogRead(sensorLDR);
  pos = map(pos, 480, 40, 75, 90);
  
  stearing.write(pos);
  delay(100);
  
  Serial.println("");
  Serial.println(pos);
  Serial.println(analogRead(A0));
  Serial.println("");
  
  //delay(1000);
}
