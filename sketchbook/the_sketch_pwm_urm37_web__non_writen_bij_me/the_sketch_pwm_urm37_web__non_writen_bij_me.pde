int temp = 0;


// Pins for ultra URM37 V3.2
int ultraData = 11;
int ultraTrigger = 10;
int ultraEnable = 9;
int ultraPower = 12;

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
  Serial.begin(9600);
}


void loop()
{
  temp = readUltra();
  Serial.println(temp);
}

