#include "URMSerial.h"
#define DISTANCE 1

URMSerial urm;

int enablepwrpin = 13;
int suplypositive = 10;
int suplynegative = 9;

int ledpin = 2;

void setup()
{
  pinMode(enablepwrpin, OUTPUT);
  pinMode(suplypositive, OUTPUT);
  pinMode(suplynegative, OUTPUT);
  pinMode(ledpin, OUTPUT);
  
  Serial.begin(9600);
  urm.begin(12,11,9600);
  
}

void loop()
{
  digitalWrite(enablepwrpin, HIGH);
  digitalWrite(suplypositive, HIGH);
  digitalWrite(suplynegative, LOW);
  
  Serial.print("Measurement: ");
  Serial.println(getMeasurement(DISTANCE));
    
  delay(100);
  
}
    int value;
    int getMeasurement(int mode)
  {
    switch(urm.requestMeasurementOrTimeout(mode,value))
    
    {
      case DISTANCE:
      
        if(value < 4){
          digitalWrite(ledpin, HIGH);
          
        }
        
        else{
           digitalWrite(ledpin, LOW); 
        }
      
      return value;     
      break;
    }
    return -1;
        
  }
