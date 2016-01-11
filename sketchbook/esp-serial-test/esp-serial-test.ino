
void setup()
{
    Serial.begin(250000);
}

void loop()
{
    for(int i = 0; i < 16; i++)
    {
        Serial.write('a');
    }
}