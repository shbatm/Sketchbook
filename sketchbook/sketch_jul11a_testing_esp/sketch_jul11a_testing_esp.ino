void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  char inbyte;
  if(Serial.available() > 0)
  {
    inbyte = Serial.read();
    // if we receive 'n'ext we send a frame.
    if(inbyte == 'n')
    {
      // say we are sending a 'f'rame.
      Serial.write('f');
      // send all the data
      for(int i = 0;i< 80;i++)
      {
        Serial.write(i);
        delay(0);
      } 
    }
  }
  delay(10);
}
