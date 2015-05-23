void setup()
{
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
    Serial.write(Serial.read());
}
