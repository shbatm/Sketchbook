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
