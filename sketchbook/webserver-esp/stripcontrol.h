#ifndef STRIPCONTROL_H
#define STRIPCONTROL_H

#include <Arduino.h>

// holds data for led control.
typedef struct {
  int pincode;
  int effect;
  int brightness;
  int varZero;
  int varOne;
  int varTwo;
  bool changed;
} stripcontrol_t;

#endif