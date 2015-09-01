//ir-sensor.ino

unsigned long previous;
unsigned long current;


#define KATHODE     A0
#define ANODE       A1
#define SENSORPIN   A2

void setup()
{
    Serial.begin(115200);
    pinMode(KATHODE, OUTPUT);
    pinMode(ANODE, OUTPUT);
    digitalWrite(ANODE, HIGH);
    digitalWrite(KATHODE, LOW);
}

void loop()
{
    Serial.println(analogRead(SENSORPIN));
}
