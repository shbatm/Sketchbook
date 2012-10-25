#include <glcd.h>
#include "fonts/allFonts.h"

glcd lcd;

gText tempDisp = gText(16,15,56,46);
gText CelciusDisp = gText(56,13,66,27);

void setup(){  
  lcd.Init();
  lcd.SelectFont(System5x7,BLACK);
  
  tempDisp.SelectFont(fixednums15x31);
  CelciusDisp.SelectFont(System5x7);
}
void loop(){
  //draw a border:
  for(int borderWidth = 0;borderWidth<5;borderWidth++){
    lcd.DrawRoundRect(borderWidth,borderWidth,127-(2*borderWidth),63-(2*borderWidth),8);
  }
  //draw the data in C
  for(int i=20;i<80;i+=5){
    tempDisp.CursorToXY(0,0);
    tempDisp.print(i);
    CelciusDisp.print('C');
    CelciusDisp.print('o');
    delay(1500);
  }
}

