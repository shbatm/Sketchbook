  #include <IOexpander.h>
  #include <Wire.h>;
  #include <inttypes.h>;
  
  IOexpander IOexp;
  
  int controleled = 13;
  
  void setup(){
    //for debuggin purposes i start a serial.
    Serial.begin(9600);
    //initiaze contact with expander. and see if it's workin.
    if(IOexp.init(0x20, MCP23016))
    Serial.println("Communication with IOexpander works!");
    else
    Serial.println("No communication with the IOexpander!!");
    //set controleled as output.
    pinMode(controleled, OUTPUT);
    //setting array 0 pins on expander to output.
    IOexp.pinModePort(0, OUTPUT);
    //and second array (array 1) to input possibly to experiment.
    IOexp.pinModePort(1, OUTPUT);
    //looking if als is good :)
    digitalWrite(controleled, HIGH);
  
  }
  
  void loop(){
    int state = HIGH;
    int state1 = LOW;
    //for the sake of testing im gonna swith every led
    //on one at a time.
    IOexp.digitalWritePort(0, state1);
    IOexp.digitalWritePort(1, state1);
    delay(500);
    IOexp.digitalWrite(0,0, state);
    delay(500);
    IOexp.digitalWrite(0,1, state);
    delay(500);
    IOexp.digitalWrite(0,2, state);
    delay(500);
    IOexp.digitalWrite(0,3, state);
    delay(500);
    IOexp.digitalWrite(0,4, state);
    delay(500);
    IOexp.digitalWrite(0,5, state);
    delay(500);
    IOexp.digitalWrite(0,6, state);
    delay(500);
    IOexp.digitalWrite(0,7, state);
    delay(500);
    Serial.println("first row was a succes!");
    delay(500);
    IOexp.digitalWrite(1,0, state);
    delay(500);
    IOexp.digitalWrite(1,1, state);
    delay(500);
    IOexp.digitalWrite(1,2, state);
    delay(500);
    IOexp.digitalWrite(1,3, state);
    delay(500);
    IOexp.digitalWrite(1,4, state);
    delay(500);
    IOexp.digitalWrite(1,5, state);
    delay(500);
    IOexp.digitalWrite(1,6, state);
    delay(500);
    IOexp.digitalWrite(1,7, state);
    Serial.println("Row 2 was a succes!");
    
  }

