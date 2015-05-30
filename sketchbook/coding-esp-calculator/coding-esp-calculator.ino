#include "stack.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char dbgStr[100];

char mathops[] = 
{
  '+',
  '-',
  '*',
  '/'
};

#define NUMMATHOPS (sizeof(mathops) * sizeof(char))

Node_t *stack = createNode();
Node_t *queue = createNode();

void printStackItems(Node_t *item)
{
  Serial.print(item->value);
  Serial.print(':');
  if(item->next == NULL)
  {
    Serial.println();
  }
}

void setup()
{  
  Wire.pins(0, 2);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.home();
  lcd.clear();
  
  stack->value = 0;
  stack->type = NONE;
  
  Serial.begin(115200);
  Serial.println();
  testingNodes();
}

void printStack(Node_t *stack)
{
  Serial.print("stack: ");
  for_item_do(stack, printStackItems);
}

char inbyte = 0;

void parseSerialInput()
{
  if(Serial.available())
  {
    inbyte = Serial.read();
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
      uint32_t num = atol(tempnum);
      
      if(stack->type == NONE)
      {
        stack->value = num;
        stack->type = INT;
      }
      else
      {
        Node_t *number = createNode();
        number->value = num;
        number->type = INT;
        push(stack, number);
      }
      
    }
    // check if incomming databyte is a letter
    if(isalpha(inbyte))
    {
    }
    // check if incomming databyte is neither a letter or digit but printable.
    // thus a special like @ or () or any thing like that.
    if(ispunct(inbyte))
    {
    }
    printStack(stack);
  }
}

unsigned long int current;

void loop()
{
  parseSerialInput();
  lcd.setCursor(0, 0);
  lcd.print(micros());
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
