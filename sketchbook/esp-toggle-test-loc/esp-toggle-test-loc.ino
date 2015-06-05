// 4 = green
// 5 = red
// 2 = blue
char leds[] = {4, 5, 2};

void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(leds[i], HIGH);
  }
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World!");
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(leds[i], HIGH);
//    delay(500);
//    digitalWrite(leds[i], LOW);
//    delay(500);
  }
}
