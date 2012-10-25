#include <Servo.h>
Servo Sweep;

int servoPin = 9;

void setup()
{
  Sweep.attach(servoPin);
}

void loop()
{
  for (int i = 0; i <=180; i=i+180)
  {
    Sweep.write(i);
    delay(1000);
  }
  
   
}
