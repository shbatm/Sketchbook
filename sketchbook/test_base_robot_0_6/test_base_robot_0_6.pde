//hier maak ik duidelijk welke namen ik geef aan pinnen, en aan variabelen.
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
//hier maak ik duidelijk wat de functie van de pinnen is die
//ik ga gebruiken, of ze een output of een input zijn.
//hier begin ik ook een verbinding met de computer, zo kan ik dan
//makkelijk foutjes op sporen in variabelen. en variabele parameters instellen.

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
 //hier vraag ik om output van de gelezen data in een serial monitor te zetten.
  Serial.println(analogRead(sensorLDR));
  
  //lees variable van de pot.
  sensorValue = analogRead(pot1);
  //zet 0-1023 om in 0-255.
  sensorValue = map(sensorValue, 0, 1023, 0, 255);
  
  //lees sensor 2 en map deze naar 0-255
  sensorValue2 = analogRead(sensorLDR);
  sensorValue2 = map(sensorValue2, 480, 50, 0, 255);
  
  // lees sensor 3 en map deze naar 0-255
  sensorValue3 = analogRead(sensorLDR2);//op dit moment is sensorLDR een variable weerstand om concept te testen.
  sensorValue3 = map(sensorValue3, 1023, 0, 0, 255);
        
  
  //kijkt of de sensor in de buurt is van een object.
  if (sensorValue2 <= 122)
  {
  
        //het eerste wat gebeurt is een stukje terug rijden.
        digitalWrite(M1, LOW);
        //en met welke snelheid dit gebeurt.
        analogWrite(E1, 254);
        //dan gebeurt dit voor x seconden lang.
        delay(1000);
        //daarna word de motor uit gezet.
        digitalWrite(M1, LOW);
        analogWrite(E1, 0);
        delay(500);
        //vervolgens worden de wielen bij gedraaid, volgens de
        //waarneming van sensorLDR2
       
        if (sensorValue3 < 112 )
        {
          //hier word naar links gestuurt als de linker sensor
         //minder licht opvangt
          digitalWrite(M2, HIGH);
          analogWrite(E2, 255);
          //de motor draait zo dat natuurlijk wel bij gedraaid word.
          digitalWrite(M1, LOW);
          analogWrite(E1, sensorValue);
        }
         
          else if(sensorValue3 > 132)
        {
          //naar rechts gestuurd, als de linker sensor minder
         // belicht is.
          digitalWrite(M2, LOW);
          analogWrite(E2, 255);
          //ook hier de motor draait zodat er natuurlijk wel bij gedraaid word.
          digitalWrite(M1, LOW);
          analogWrite(E1, sensorValue);
        }
        
        delay(1000);
  }
   
   //vervolgens het sstuur recht zetten en door rijden.
    else if (sensorValue2 > 122)
    {
      //zet het stuur mechanisme uit, anders blijft het constant aan blijkbaar :S
      digitalWrite(M2, HIGH);  
      //signaal word uitgezet. dat word weer gegeven met een 0.
      analogWrite(E2, 0);
       
       //laat de motor gewoon door draaien als er geen obstakel is gevonden.
       digitalWrite(M1,HIGH);
       analogWrite(E1, sensorValue);
       
       delay(30);
    }   
     
}
