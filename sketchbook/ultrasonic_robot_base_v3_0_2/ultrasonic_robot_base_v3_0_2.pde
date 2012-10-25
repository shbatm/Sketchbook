int temp;
#include <Servo.h>
Servo myservo;
//pins for the motor shield.
int M1 = 4;
int E1 = 6;

int potpin = A0; // speed controle variable, controled by a pot.
int speedvalue; //value to be populated and mapped


// Pins for ultra URM37 V3.2
int ultraData = 11;
int ultraTrigger = 10;
int ultraEnable = 9;
int ultraPower = 12;
int servopin = 8;

int readUltra(){
  int ultraValue = 0;
  int timecount = 0;
  int val = 0;

  // Wake up and get ready for sensing
   pinMode(ultraPower, OUTPUT);
  digitalWrite(ultraPower, HIGH);  // Set to HIGH to provide 5V power
  pinMode(ultraEnable, OUTPUT);
  digitalWrite(ultraEnable, HIGH); // Set Enable Pin to HIGH
  pinMode(ultraTrigger, OUTPUT);   // Switch signalpin to output
  pinMode(ultraData,INPUT);
  delay(200); //Give sensor some time to start up --Added By crystal  from Singapo, Thanks Crystal.
 
 
  /* Send high-low-high pulse to activate the trigger pulse of the sensor
  * -------------------------------------------------------------------
  */
  digitalWrite(ultraTrigger, HIGH); // Send High pulse
  delayMicroseconds(500);
  digitalWrite(ultraTrigger, LOW); // Send low pulse
  delayMicroseconds(200);
  digitalWrite(ultraTrigger, HIGH); // Send High pulse
  delayMicroseconds(200);
 
 
  /* Listening for echo pulse
  * -------------------------------------------------------------------
  */
  // Loop until pin reads a high value
  do{
       val = digitalRead(ultraData);
       //Serial.println("BBB");
     }while(val == HIGH);
  // Loop until pin reads a high value
  do{
     val = digitalRead(ultraData);
     timecount ++;            // Count echo pulse time
     delayMicroseconds(50);
     }while(val == LOW);
    ultraValue = timecount; // Append echo pulse time to ultraValue

// Turn the lights out when you leave...
  digitalWrite(ultraTrigger,LOW);
  digitalWrite(ultraEnable,LOW);
  digitalWrite(ultraPower,LOW);

  return ultraValue;
}



void setup()
{
  pinMode(servopin, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(potpin, INPUT);
  myservo.attach(8);
  Serial.begin(9600);
}


void loop(){
  
      {
      
      speedvalue = analogRead(potpin);// giving speedvalue the value/variable of potpin.
      speedvalue = map(speedvalue, 1023, 0, 255, 0);// then map it so that 1023 is equal to 255.
      
         { 
          digitalWrite(M1, HIGH);// forward motion, an LOW is backward motion of the motor.
          analogWrite(E1, speedvalue);//speedvalue is a value between 0 and 255 that controles speed, 0 is no speed and 255 is max speed.
          
            Serial.println("value the pot gives:");
            Serial.println(analogRead(potpin));
            Serial.println("");
            Serial.println(speedvalue);
            Serial.println("value on speedvalue should be between 0 and 255");
            Serial.println("");
         }
    }
    
    {
      temp = readUltra();
      if (temp <= 50){
     
      int brightness;
      temp = readUltra();
    
        brightness = readUltra();
        brightness = map(brightness, 1, 50, 45, 90);
        
        myservo.write(brightness);
        
      //Serial.println("Distance");
      //Serial.println(temp);
      Serial.println("brightness");
      Serial.println(brightness);
      Serial.println("");
    }  
     
      else {
      myservo.write(90);
      }
     
    }
}

