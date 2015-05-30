#ifndef STACK_H
#define STACK_H

#include <Arduino.h>
extern "C" 
{
  #include "ets_sys.h"
  #include "osapi.h"
  #include "os_type.h"
  #include "user_interface.h"
  #include "espconn.h"
  #include "mem.h"
  #include "gpio.h"
  #include "user_config.h"
}

#include <assert.h>

enum {NONE, CHAR, INT, OPP, OPEN_PAREN, CLOSE_PAREN};

struct _node
{
  int value;
  uint8_t type;
  struct _node *next;
};

typedef struct _node Node_t;

void testingNodes(void);

Node_t *createNode();
Node_t *createNode(int);
void deleteNode(Node_t **);
unsigned int nodeLength(Node_t *);
void appendNode(Node_t *, Node_t *);
void push(Node_t *, Node_t *);
Node_t *pop(Node_t *);
void for_item_do(Node_t *, void(*)(Node_t *));

#endif
