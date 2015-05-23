#include "stack.h"

// create a single node to start the list with
Node_t createNode()
{
  Node_t root = (Node_t)new(Node_t);
  root->next = NULL;
  return root;
}

// create a list with n nodes.
Node_t createNode(unsigned int len)
{
  Node_t root = createNode();
  Node_t nodes = root;
  int i;
  // len-1 because we already initialized a root node.
  for(i = 0;i < (len-1); i++)
  {
    nodes->next = createNode();
    nodes = nodes->next;
  }
  return root;
}



// append a node to the end of the list.
void appendNode(Node_t list, Node_t item)
{
  //keep a copy of the first node.
  Node_t node = list;
  //go to the last one
  while(node->next != NULL)
  {
    node = node->next;
  }
  node->next = item;
  item->next = NULL;
}

// count the number of nodes in a list.
unsigned int listLen(Node_t list)
{
  unsigned int len = 0;
  Node_t head = list;
  while(head != NULL)
  {
    len++;
    head = head->next;
  }
  return len;
}
