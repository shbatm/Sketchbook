#include "URMSerial.h"
// The measurement we're taking
#define DISTANCE 1
URMSerial urm;

int testled = 13;

void setup() {

  pinMode(testled, OUTPUT);

  Serial.begin(9600);                  // Sets the baud rate to 9600
  urm.begin(2,3,9600);                 // RX Pin, TX Pin, Baud Rate
  Serial.println("URM37 Library by Miles Burton - Distance. Version 2.0");   // Shameless plug 
  Serial.println(getMeasurement(DISTANCE));
}

void loop()
{
  Serial.println("Distance measurement");
  Serial.println(getMeasurement(DISTANCE));
  
 delay(1000);

}
int value; // This value will be populated
int getMeasurement(int mode)
{
  // Request a distance reading from the URM37
  switch(urm.requestMeasurementOrTimeout(mode, value)) // Find out the type of request
  {
  case DISTANCE: // Double check the reading we recieve is of DISTANCE type
    //Serial.println("Value");
    //Serial.println(value); // Fetch the distance in centimeters from the URM37
    //Serial.println("");
    delay(1000);
      
    if (value < 30)
    {
      digitalWrite(testled, HIGH);
    }
    else if (value > 30)
    {
      digitalWrite(testled, LOW);
    }
    
    return value;
    break;
    
  }
}



