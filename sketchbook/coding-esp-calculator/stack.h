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

struct _node
{
  int value;
  struct _node *next;
};

typedef struct _node Node_t;

Node_t *createNode();

Node_t *createNode(int);

void deleteNode(Node_t **);
unsigned int nodeLength(Node_t *);

void for_item_do(Node_t *, void(*)(Node_t *));

#endif
