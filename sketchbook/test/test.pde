int sensorPin = A2;    // Analog input pin to pot
int ledPin = 6;      // PWM pin to LED
int sensorValue = 0;  // variable to store pot value


void setup() {
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);  
  // converts 0-1023 to 0-255
  sensorValue = map(sensorValue, 160, 16, 0, 255);   
  // outputs PWM signal to LED
  analogWrite(ledPin, sensorValue);
}

