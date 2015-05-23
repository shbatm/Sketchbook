#ifndef STACK_H
#define STACK_H

#include <stdio.h>

struct _node
{
  int x;
  struct _node *next;
};

typedef struct _node* Node_t;

// create a single node to start the list with
Node_t createNode();

// create a list with n nodes.
Node_t createNode(unsigned int len);

// delete node or list of nodes.
void deleteNode(Node_t node);

// append a node to the end of the list.
void appendNode(Node_t list, Node_t item);

// count number of nodes in list.
unsigned int listLen(Node_t list);

// pop item from list and return item. sets last item pointer to null
Node_t pop(Node_t list);

// pushes item to list. 
Node_t push(Node_t list, Node_t item);

#endif
