#include "stack.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char dbgStr[100];

enum {CHAR, INT, OPP, OPEN_PAREN, CLOSE_PAREN};

void countItems(Node_t *item)
{
  static int i = 0;
  i++;
  Serial.print(i);
  Serial.print(", ");
  // check if it's the last item in the list.
  // if so print a newline.
  if(item->next == NULL)
    Serial.println();
}

void setValue(Node_t *item)
{
  item->value = random(0, 2000);
  Serial.print(item->value);
  Serial.print(", ");
  // check if it's the last item in the list.
  // if so print a newline.
  if(item->next == NULL)
    Serial.println();
}

void printValue(Node_t *item)
{
  Serial.print(item->value);
  Serial.print(", ");
  // check if it's the last item in the list.
  // if so print a newline.
  if(item->next == NULL)
    Serial.println();
}

void setup()
{
  Wire.pins(0, 2);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.home();
  lcd.clear();
  randomSeed(millis());
  Serial.begin(115200);
  Serial.println();
  
  Serial.println("creating node.");
  Node_t *node = createNode();
  Serial.print("node created: ");
  Serial.println(node != NULL ? "True" : "False");
  Serial.println("assigning value.");
  node->value = 10;
  Serial.print("node->value: ");
  Serial.println(node->value);
  Serial.println("deleting node."); 
  deleteNode(&node);
  Serial.print("deleted node: ");
  Serial.println(node == NULL ? "True" : "False");
  
  Serial.println();
  Serial.print("creating a list: ");
  node = createNode(10);
  Serial.println(nodeLength(node));
  
  //should print the amount of items.
  Serial.print("for item do a count: ");
  for_item_do(node, countItems);
  Serial.print("for item assign value: ");
  for_item_do(node, setValue);
  Serial.print("for item print value: ");
  for_item_do(node, printValue);
  
  Serial.println("deleting list.");
  deleteNode(&node);
  Serial.print("list deleted: ");
  Serial.println(node == NULL ? "True" : "False");
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print(millis());
  delay(100);
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
