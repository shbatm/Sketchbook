#include "stack.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char dbgStr[100];

Node_t *stack = createNode();

void printStackItems(Node_t *item)
{
  Serial.print(item->value);
  Serial.print(' ');
  if(item->next == NULL)
  {
    Serial.println();
  }
}

void setup()
{
  stack->value = 0;
  stack->type = NONE;
  
  Wire.pins(0, 2);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.home();
  lcd.clear();
  
  Serial.begin(115200);
  Serial.println();
}

void printStack(Node_t *stack)
{
  for_item_do(stack, printStackItems);
}

char inbyte = 0;

void parseSerialInput()
{
  inbyte = Serial.read();
  if(Serial.available() > 0)
  {
    if(isdigit(inbyte))
    {
      char tempnum[64];
      int i = 0;
      tempnum[i++] = inbyte;
      while((Serial.available() > 0) && isdigit(inbyte))
      {
        inbyte = Serial.read();
        tempnum[i] = inbyte;
        tempnum[++i] = '\0';
      }
      Serial.print(strlen(tempnum));
      Serial.print(":");
      Serial.println(tempnum);

      uint32_t num = atol(tempnum);
      Serial.print(sizeof num);
      Serial.print(":");
      Serial.println(num);
    }
    // check if incomming databyte is a letter
    else if(isalpha(inbyte))
    {
    }
    // check if incomming databyte is neither a letter or digit but printable.
    // thus a special like @ or () or any thing like that.
    else if(ispunct(inbyte))
    {
    }
  }
}

void loop()
{
  parseSerialInput();
//  printStack(stack);
  lcd.setCursor(0, 0);
  lcd.print(millis());
}

//void setup()
//{
//  Wire.pins(0, 2);
//  lcd.begin(16, 2);
//  lcd.backlight();
//  lcd.home();
//  lcd.clear();
//  Serial.begin(115200);
//}
//
//void loop()
//{
//  if(Serial.available() > -1)
//  {
//    char inbyte = Serial.read();
//    if(isdigit(inbyte))
//    {
//      char tempnum[64] = {0};
//      int i = 0;
//      while((Serial.available() > 0) && isdigit(inbyte))
//      {
//        tempnum[i] = inbyte;
//        tempnum[i+1] = '\0';
//        i++;
//        inbyte = Serial.read();
//      }
//      Serial.print(strlen(tempnum));
//      Serial.print(":");
//      Serial.println(tempnum);
//
//      uint32_t num = atoi(tempnum);
//      Serial.print(sizeof num);
//      Serial.print(":");
//      Serial.println(num);
//    }
//    // check if incomming databyte is a letter
//    else if(isalpha(inbyte))
//    {
//    }
//    // check if incomming databyte is neither a letter or digit but printable.
//    // thus a special like @ or () or any thing like that.
//    else if(ispunct(inbyte))
//    {
//    }
//  }
//  lcd.setCursor(0, 0);
//  lcd.print(millis());
//}
