//mindflex-esp8266.ino

#include <Ticker.h>

int mindflex_led = 16;
Ticker blinkTick;

void blinker()
{
    static bool state;
    state = !state;
    digitalWrite(mindflex_led, state);
}

void setup() {
  // setup status led.
  pinMode(mindflex_led, OUTPUT);
  blinkTick.attach(1, blinker);

  // setup coms.
  Serial.begin(9600);
}

void loop()
{
    if(Serial.available())
    {
        Serial.write(Serial.read());
    }
}