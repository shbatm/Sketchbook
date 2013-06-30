#include <multiCameraIrControl.h>

Pentax IR_pin(9);

uint8_t shutter = 7;
uint8_t toggle_focus = 6;

void setup()
{
  pinMode(toggle_focus, INPUT);
  pinMode(shutter, INPUT);
}
inline void calledFunc()
{
  if(digitalRead(shutter))
  {
    IR_pin.shutterNow();
    delay(10);
  }
  else if(digitalRead(toggle_focus))
  {
    IR_pin.toggleFocus();
    delay(10);
  }
}
void loop(){
  calledFunc();
}
