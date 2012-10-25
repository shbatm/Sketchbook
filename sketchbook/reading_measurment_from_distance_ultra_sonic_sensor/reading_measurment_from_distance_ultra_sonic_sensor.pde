#include "URMSerial.h"
// het type meeting, afstand dus.
#define DISTANCE 1

URMSerial urm;

void setup() {
  Serial.begin(9600);                  // Sets the baud rate to 9600
  urm.begin(6,7,9600);                 // RX Pin, TX Pin, Baud Rate
  
}

void loop() {
  Serial.print("Measurement: ");
  Serial.println(getMeasurement(DISTANCE));  // Output measurement
  delay(1000);
}

int value; // This value will be populated
int getMeasurement(int mode)

{
  //vraag afstand meeting van de URM37
  switch(urm.requestMeasurementOrTimeout(mode, value))
  {
  case DISTANCE: // Double check the reading we recieve is of DISTANCE type
    Serial.println(value); // Fetch the distance in centimeters from the URM37
    return value;
    break;

  }

  
}

