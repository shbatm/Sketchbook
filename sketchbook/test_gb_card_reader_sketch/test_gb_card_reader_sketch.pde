    //first set up the settings to use the i/o expander.
  #include <IOexpander.h>
  #include <Wire.h>;
  #include <inttypes.h>;
  
  IOexpander IOexp;
  
  //pins for read/write.
  int rdPin = 12;
  int wrPin = 13;
  
  void setup (){
     //serial for debugging and reading from.
     //high baud because of python script.
     Serial.begin(57600);
     //looking if there is communication with the i/o expander.
     if(IOexp.init(0x20, MCP23016))
        Serial.println("Communication with IOexpander works!");
    else
        Serial.println("No communication with the IOexpander!!");
     //setting read/write pins as output.
     pinMode(rdPin, OUTPUT);
     pinMode(wrPin, OUTPUT);
     //setting pins on i/o expander to output.
     IOexp.pinModePort(0, OUTPUT);
     IOexp.pinModePort(1, OUTPUT);
     
     //setting data lines as input, so we can read them.
     pinMode(2, INPUT);
     pinMode(3, INPUT);
     pinMode(4, INPUT);
     pinMode(5, INPUT);
     pinMode(6, INPUT);
     pinMode(7, INPUT);
     pinMode(8, INPUT);
     pinMode(9, INPUT);
  }
  
  void loop(){
    unsigned int addr = 0; // Variable ranges between 0 to 65,536 (memory size)
      
      digitalWrite(rdPin, LOW);//setting RD to 0 to read.
      digitalWrite(wrPin, HIGH);//setting WR to high so it know we want to read not write.
      
        Serial.println("START"); //Send the start command
        
        //Read addresses 0000h to 3FFF, i.e. ROM bank 0
        for (addr = 0; addr <= 0x3FFF; addr++){
          int var = addr;
          IOexp.digitalWrite(0,0, HIGH && (var & 0x0001));
          IOexp.digitalWrite(0,1, HIGH && (var & 0x0002));
          IOexp.digitalWrite(0,2, HIGH && (var & 0x0004));
          IOexp.digitalWrite(0,3, HIGH && (var & 0x0008));
          IOexp.digitalWrite(0,4, HIGH && (var & 0x0010));
          IOexp.digitalWrite(0,5, HIGH && (var & 0x0020));
          IOexp.digitalWrite(0,6, HIGH && (var & 0x0040));
          IOexp.digitalWrite(0,7, HIGH && (var & 0x0080));
          IOexp.digitalWrite(1,0, HIGH && (var & 0x0100));
          IOexp.digitalWrite(1,1, HIGH && (var & 0x0200));
          IOexp.digitalWrite(1,2, HIGH && (var & 0x0400));
          IOexp.digitalWrite(1,3, HIGH && (var & 0x0800));
          IOexp.digitalWrite(1,4, HIGH && (var & 0x1000));
          IOexp.digitalWrite(1,5, HIGH && (var & 0x2000));
          IOexp.digitalWrite(1,6, HIGH && (var & 0x4000));
          IOexp.digitalWrite(1,7, HIGH && (var & 0x8000));
        
        
          delayMicroseconds(50);
          
          {
          int D1;
          int D2;
          int D3;
          int D4;
          int D5;
          int D6;
          int D7;
          int D8;
          int Data;
          
          D1 = digitalRead(2);
          D2 = digitalRead(3);
          D3 = digitalRead(4);
          D4 = digitalRead(5);
          D5 = digitalRead(6);
          D6 = digitalRead(7);
          D7 = digitalRead(8);
          D8 = digitalRead(9);
          
         
          //Serial.println(Data);
           
           Serial.print(D1);
           Serial.print(D2);
           Serial.print(D3);
           Serial.print(D4);
           Serial.print(D5);
           Serial.print(D6);
           Serial.print(D7);
           Serial.print(D8);
           Serial.println(" ");
           
        }
        
          }
          Serial.println("END");
        while(1);
        }
          
//          byte bval = 0;
//          for (int z = 9; z <= 2; z--) {
//            if (digitalRead(z) == HIGH) {
//              bitWrite(bval, (z-2), HIGH);
//              
//            }
//          }
//          Serial.println(bval, DEC);
//          
//        }
//          {
//            Serial.println("END");
//            while(1);
//          }
  
