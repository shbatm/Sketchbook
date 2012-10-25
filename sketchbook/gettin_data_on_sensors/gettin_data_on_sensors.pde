
int sensorpin = A0;
int ledPin = 13;
int sensorpinx;

void setup()
{
  
  pinMode(sensorpin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9800);
  
}

void loop()
{
  sensorpinx = analogRead(sensorpin);
  sensorpinx = map(sensorpinx, 100, 850, 0, 255);
  
  digitalWrite(ledPin, HIGH);
  delay(100);
  
  digitalWrite(ledPin, LOW);
  delay(100);
   Serial.println(sensorpinx);
   Serial.println(analogRead(sensorpin));
   delay(1000);
   Serial.println("");
}

