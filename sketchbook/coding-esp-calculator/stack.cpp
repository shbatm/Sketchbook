#include "stack.h"

Node_t *createNode()
{
  // function to create a new node.
  Node_t *root = (Node_t *)os_malloc(sizeof(Node_t));
  root->next = NULL;
  return root;
}

Node_t *createNode(int amount)
{
  // function to create multiple nodes in a list like structure.
  Node_t *root = createNode();
  Node_t *head = root;
  for(int i = 0; i < (amount - 1);i++)
  {
    head->next = createNode();
    head = head->next;
  }
  head->next = NULL;
  return root;
}

void deleteNode(Node_t **root)
{
  //if next node == NULL it's not a list.
  if((*root)->next == NULL)
  {
    // and free a single node.
    os_free(*root);  
    *root = NULL;
  }
  else
  {
    // else we loop over the list. going to the next.
    // and freeing the previous one.
    Node_t **temp = NULL;
    while((*root) != NULL)
    {
      temp = root;
      *root = (*root)->next;
      os_free(*temp);
    }
    *temp = NULL;
    *root = NULL;
  }
}

void appendNode(Node_t *root, Node_t *node)
{
  //traverse the list.
  Node_t *head = root;
  while(head->next != NULL)
    head = head->next;
  //append node to the end.
  head->next = node;
}

unsigned int nodeLength(Node_t *node)
{
  // counts the number of nodes in a list.
  int i = 0;
  Node_t *root = node;
  while(root != NULL)
  {
    i++;
    root = root->next;
  }
  return i;
}

void push(Node_t *root, Node_t *node)
{
  appendNode(root, node);
}

Node_t *pop(Node_t *root)
{
  if(nodeLength(root) == 1)
  {
    return root;
  }
  else if(nodeLength(root) > 1)
  {
    // this one is tricky
    Node_t *node = NULL;
    Node_t *head = root;
    // we traverse the list be stop one before last
    while(head->next->next != NULL)
    {
      head = head->next;
    }
    // we store the last one.
    node = head->next;
    // remove the link to it
    head->next = NULL;
    // and return the stored link
    // to be used in another node.
    return node;
  }
  else
  {
    Serial.println("Error in pop func, length ! >= 1");
  }
}

void for_item_do(Node_t *root, void(*func)(Node_t *))
{
  // goes over every node,
  // and passes that to a function handler.
  int i = 0;
  for(Node_t *head = root;head != NULL; head = head->next)
  {
    func(head);
  }
}

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

void testingNodes(void)
{
  randomSeed(millis());
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
  Serial.println("appending node with value 42");
  Node_t *newNode = createNode();
  newNode->value = 42;
  appendNode(node, newNode);
  Serial.print("printing values in list: ");
  for_item_do(node, printValue);
  
  Serial.print("poping: ");
  Node_t *anode = pop(node);
  Serial.println(anode->value);
  Serial.print("printing values in list: ");
  for_item_do(node, printValue);
  
  Serial.println("deleting list.");
  deleteNode(&node);
  Serial.print("list deleted: ");
  Serial.println(node == NULL ? "True" : "False");
  Serial.print("new node deleted: ");
  Serial.println(newNode == NULL ? "True" : "False");
  Serial.print("newNode->value: ");
  Serial.println(newNode->value);
  Serial.println("deleting newnode.");
  deleteNode(&newNode);
  Serial.print("newnode deleted?: ");
  Serial.println(newNode == NULL ? "True" : "False");
  // accessing a node after it's deleted causes the wdt to go reset the esp.
  // i gues it works! :)
  //Serial.print("newNode->value: ");
  //Serial.println(newNode->value);
}
