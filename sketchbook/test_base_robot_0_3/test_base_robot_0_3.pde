int E2 = 6;
int M2 = 7;
int E1 = 5;
int M1 = 4;

int sensorValue = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;
int pot1 = A2;
int sensorLDR = A1;
int sensorLDR2 = A3;

void setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(pot1, INPUT);
  pinMode(sensorLDR, INPUT);
  pinMode(sensorLDR2, INPUT);
  Serial.begin(9800);
}

void loop()
{
  Serial.println(analogRead(sensorLDR));
  //lees variable van de pot.
  sensorValue = analogRead(pot1);
  //zet 0-1023 om in 0-255.
  sensorValue = map(sensorValue, 0, 1023, 0, 255);
  
  sensorValue2 = analogRead(sensorLDR);
  sensorValue2 = map(sensorValue2, 0, 1023, 0, 255);
  
  //kijkt of de sensor in de buurt is van een object.
  if (sensorValue2 <= 122)
  {
  
        //als sensorValue2 kleiner is dan 180, dan worden de poorten laag, en gaan de wiellen achteruit.
        digitalWrite(M1, LOW);
        // bepaalt aan de hand van sensor1, hoe snel hij achteruit moet gaan.
        analogWrite(E1, sensorValue);
        //doe dit voor 3,5 seconden.
        delay(2000);
        
        // lees sensor 3 en map deze naar 0-255
        sensorValue3 = analogRead(sensorLDR2);//op dit moment is sensorLDR een variable weerstand om concept te testen.
        sensorValue3 = map(sensorValue3, 1023, 0, 0, 255);
        
        if (sensorValue3 < 122 )
        {
          digitalWrite(M2, HIGH);
          analogWrite(E2, 255);
        }
         
          else if(sensorValue3 > 122)
        {
          digitalWrite(M2, LOW);
          analogWrite(E2, 255);
        }
        
        delay(1000);
  }
   
   
    else if (sensorValue2 > 122)
    {
      //zet de poorten van de motorshield hoog. zodat je via de andere poorten de stroom kan varieren.
      digitalWrite(M2, HIGH);
  
      //en bepaalt aan de hand van sensor1, hoe snel hij vooruit moet gaan.
  
      analogWrite(E2, 0);
    }
  
  
  //als sensor2 niet kleiner is dan 180 gaan de wiellen achteruit.
  //en zend het PWM signaal naar het motorshield.
  else
  {
  //zet de poorten van de motorshield hoog. zodat je via de andere poorten de stroom kan varieren.
  digitalWrite(M1, HIGH);
  
  //en bepaalt aan de hand van sensor1, hoe snel hij vooruit moet gaan.
  
  analogWrite(E1, sensorValue);
  }
   delay(300);
}
