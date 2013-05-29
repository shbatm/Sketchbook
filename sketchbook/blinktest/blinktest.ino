int pins[1] = {13};

void setup()
{
  pinMode(pins[0], OUTPUT);
  digitalWrite(pins[0], HIGH);
}

void loop()
{
  digitalWrite(pins[0], HIGH);
  delay(500);
  digitalWrite(pins[0], LOW);
  delay(500);
}
