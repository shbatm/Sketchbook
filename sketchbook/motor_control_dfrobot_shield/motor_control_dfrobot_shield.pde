int E1 = 6;
int M1 = 7;
int E2 = 5;
int M2 = 4;

int sensorValue ;
int sensorPin = A2;

void setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop()
{
  //zet de poorten van de motorshield hoog. zodat je via de andere poorten de stroom kan varieren.
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  //lees variable van de pot.
  sensorValue = analogRead(sensorPin);
  //zet 0-1023 om in 0-255.
  sensorValue = map(sensorValue, 0, 1023, 0, 255);
  //zend het PWM signaal naar het motorshield.
  analogWrite(E2, sensorValue);
  analogWrite(E1, sensorValue);
  
}
