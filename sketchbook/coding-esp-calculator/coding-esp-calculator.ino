#include "stack.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char dbgStr[100];

typedef struct {
  char op;
  int percendence;
} operator_t;

operator_t ops[] =
{
  {'+', 0},
  {'-', 1},
  {'*', 2},
  {'/', 3}
};

#define NUMOPS (sizeof(ops) / sizeof(operator_t))

Node_t *stack = NULL;
Node_t *queue = NULL;

void printStackItems(Node_t *item)
{
  Serial.print(item->value);
  Serial.print(':');
  if (item->next == NULL)
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

  Serial.begin(115200);
  Serial.println();
  testingNodes();
}

void printStack(Node_t *stack)
{
  Serial.print("stack: ");
  for_item_do(stack, printStackItems);
  Serial.println();
}

char inbyte = 0;

void parseSerialInput()
{
  if (Serial.available())
  {
    inbyte = Serial.read();
    if (isdigit(inbyte))
    {
      char tempnum[64];
      int i = 0;
      tempnum[i++] = inbyte;
      while ((Serial.available() > 0) && isdigit(inbyte))
      {
        inbyte = Serial.read();
        tempnum[i] = inbyte;
        tempnum[++i] = '\0';
      }
      uint32_t num = atol(tempnum);
      
      Node_t *number = createNode();
      number->value = num;
      number->type = INT;
      
      // is there anything on the stack?
      if(stack == NULL)
      {
        // if not make first node number.
        Serial.println("pushing to stack since stack NULL. ");
        stack = number;
        Serial.println(stack == NULL ? "True" : "False");
      }
      else
      {
        // if stack do your magic parsing.
        Node_t *poped = pop(stack);
        Serial.println(poped == NULL ? "True" : "False");
        Serial.println(stack == NULL ? "True" : "False");
        Serial.print("poped->value: ");
        Serial.println(poped->value);
        Serial.println(poped->type == INT ? "type==INT" : "type!=INT");
        if(poped->type == INT)
        {
          push(stack, poped);
          push(stack, number);
        }
      }
    }
    // check if incomming databyte is a letter
    if (isalpha(inbyte))
    {
    }
    // check if incomming databyte is neither a letter or digit but printable.
    // thus a special like @ or () or any thing like that.
    if (ispunct(inbyte))
    {
//      Serial.println(NUMOPS);
//      for(int i = 0;i<NUMOPS;i++)
//      {
//        Serial.println(ops[i].op);
//      }
    }
    printStack(stack);
  }
}

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
