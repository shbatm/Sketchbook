#include <IOlcd.h>
#include <Wire.h>
#include <PS2Keyboard.h>

IOlcd lcd;
PS2Keyboard keyboard;
const int DataPin = 3;
const int IRQpin = 2;

int menuIndex = 1;
char inputKeyboard;

//char menuItem[17] = {"Select Program:|"};
//char subMenuItem[17] = {"1.Serial Monitor"};

char Menu[9] [17] ={
  {"Select Program: "},
  {"1.Serial Monitor"},
  {"2. Test entry   "},
  {"3. another test "},
  {"4. another space"},
  {"5. another space"},
  {"6. another space"},
  {"7. another space"},
  {"8. another space"}
};

void setup(){
  lcd.init(0x20, MCP23016);
  keyboard.begin(DataPin, IRQpin);
  lcd.begin(16,2);
  lcd.home();
  lcd.clear();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print(Menu[0]);
  if(keyboard.available()){
    inputKeyboard = keyboard.read();
    switch(inputKeyboard){
      case PS2_DOWNARROW:
      menuIndex++;
      if(menuIndex==9){
        menuIndex = 1;
      }
      break;
      case PS2_UPARROW:
      menuIndex--;
      if(menuIndex<1){
        menuIndex = 8;
      }
      break;
      default:
      break;
    }
  }
  else{
    lcd.setCursor(0,1);
    lcd.print(Menu[menuIndex]);
  }
}
