  #include "URMSerial.h"
  // The measurement we're taking
  #define DISTANCE 1
  URMSerial urm;
  
  //define the pins i use.
  int E2 = 6;
  int M2 = 7;
  int E1 = 5;
  int M1 = 4;
  
  
  int speedpot = A0;
  int speedvalue = 0;
  int directionpot = A1;
  int directionvalue = 0;
  
  
  
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
        
        directionvalue = analogRead(directionpot);
        directionvalue = map(directionvalue, 1023, 0, 0, 255);
        
        speedvalue = analogRead(speedpot);
        speedvalue = map(speedvalue, 1023, 0, 255, 0);
        
        //kijken of de sonische sensor een object op een afstand van 30cm zit.
      if (value < 30)
        {
          //het eerste wat gebeurt is een stukje terug rijden.
        digitalWrite(M1, LOW);
        //en met welke snelheid dit gebeurt.
        analogWrite(E1, speedvalue);
        //dan gebeurt dit voor x seconden lang.
        delay(1000);
        //daarna word de motor uit gezet.
        digitalWrite(M1, LOW);
        analogWrite(E1, 0);
        delay(100);
        
        
        //vervolgens worden de wielen bij gedraaid, volgens de
        //waarneming van richtingspot
        if (directionvalue < 122)
          {
            
            
            
          }
          
        else if (directionvalue > 122)
          {
            
            
            
          }
           
          
        }
          
         
         
          else if (value > 30) {
            
            
            
          }
      
      return value;
      break;
      
    }
  }
  
  

