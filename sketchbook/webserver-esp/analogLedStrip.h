#ifndef ANALOGLEDSTRIP_H
#define ANALOGLEDSTRIP_H

#include <Arduino.h>

#define RED 5
#define GREEN 4
#define BLUE 2

// setup pin related things.
void setupAnalogStrip();
/*
    write a red green blue value,
    corresponding to it's brightness,
    with value range 0-255.
*/
void writeRgb(int, int, int);

#endif