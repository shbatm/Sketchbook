#ifndef STRIPCONTROL_H
#define STRIPCONTROL_H

// holds data for led control.
typedef struct {
  int pincode;
  int effect;
  int brightness;
  int varZero;
  int varOne;
  int varTwo;
} stripcontrol_t;

#endif