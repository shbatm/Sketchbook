void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.write(128);
    for(int i = 0; i < 80; i++)
    {
        Serial.write(i);
    }
    Serial.write(129);
    delay(100);
}
