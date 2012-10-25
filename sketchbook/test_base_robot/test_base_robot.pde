int E1 = 6;
int M1 = 7;
int E2 = 5;
int M2 = 4;

int sensorValue ;
int sensorValue2 ;
int pot1 = A2;
int sensorLDR = A1;

void setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(pot1, INPUT);
  pinMode(sensorLDR, INPUT);
}

void loop()
{
  //lees variable van de pot.
  sensorValue = analogRead(pot1);
  //zet 0-1023 om in 0-255.
  sensorValue = map(sensorValue, 0, 1023, 0, 255);
  
  sensorValue2 = analogRead(sensorLDR);
  sensorValue2 = map(sensorValue2, 710, 960, 0, 255);
  
  //kijkt of de sensor in de buurt is van een object.
  if (sensorValue2 <= 180)
  {
  //als sensor2 kleiner is dan 400, dan worden de poorten laag, en gaan de wiellen achteruit.
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  // bepaalt aan de hand van sensor1, hoe snel hij achteruit moet gaan.
  analogWrite(E2, sensorValue);
  analogWrite(E1, sensorValue);
  }
  //als sensor2 niet kleiner is dan 400 gaan de wiellen achteruit.
  //en zend het PWM signaal naar het motorshield.
  else
  {
  //zet de poorten van de motorshield hoog. zodat je via de andere poorten de stroom kan varieren.
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  //en bepaalt aan de hand van sensor1, hoe snel hij vooruit moet gaan.
  analogWrite(E2, sensorValue);
  analogWrite(E1, sensorValue);
  }
}
