extern "C" 
{
  #include <ets_sys.h>
  #include <osapi.h>
  #include <stdio.h>
}

#include "stack.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char dbgStr[100];

Node_t stack;

void setup()
{
  Wire.pins(0, 2);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.home();
  lcd.clear();
  Serial.begin(115200);
  
  Node_t node = createNode();
  if(node == NULL)
  {
    Serial.println("Not enough memory to allocate node");
    for(;;);
  }
  node->x = 10;
  os_sprintf(dbgStr, "node->x = %d", node->x);
  Serial.println(dbgStr);
  
  for(int i = 0; i< 15; i++)
  {
    Node_t next_node = createNode();
    next_node->x = i;
    appendNode(node, next_node);
  }
  for(Node_t head = node; head != NULL; head = head->next)
  {
    Serial.print(head->x);
    Serial.print(" ");
  }
  Serial.println("");
  
  unsigned int nodes = listLen(node);
  os_sprintf(dbgStr, "list length: %d", nodes);
  Serial.println(dbgStr);
  delete(node);
  
  Node_t node_list = createNode(10);
  if(node_list == NULL)
  {
    Serial.println("Not enough memory to allocate list");
    for(;;);
  }
  
  Node_t head;
  int index = 0;
  Serial.println("assigning values");
  for(head = node_list; head != NULL; head = head->next)
  {
    head->x = index++;
  }
  Serial.println("reading values");
  for(head = node_list; head != NULL; head = head->next)
  {
    Serial.print(head->x);
  }
  Serial.println("");
}

void loop()
{
  lcd.setCursor(0, 1);
  lcd.print(millis());
  delay(1000);
}
