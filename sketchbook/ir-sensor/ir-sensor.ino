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

    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    
    digitalWrite(3, HIGH);
    digitalWrite(5, LOW);
}

void loop()
{
    Serial.println(analogRead(SENSORPIN));
}
