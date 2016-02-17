/*
    Author: Duality
    Git: https://github.com/Duality4Y

    these functions can be used to store things in eeprom or flash.
    depending on your platform.
    It works aslong as your platform implements eeprom functions, 
    as Arduino Implements them.

    obviously when you make a settingsStore/settingsLoad funcion,
    In which you actually specify what is store and loaded,
    you'll have to load everything in the same order as you store them!
    Always store magic string first.
    Always call storeDone() after everything you want to store.

*/

#ifndef __SETTINGSSTORE_H__
#define __SETTINGSSTORE_H__

#include <Arduino.h>
#include <EEPROM.h>

// constant for setting how much to put asside for storage.
#define EEPROMSIZE 1024

// setup requirements for libraries used.
void setupSettingsStore();

// store a string.
void storeString(String, int&);
// store a int.
void storeInt(int, int&);
// store's a magic word to check against.
void storeMagicStr(int&);
// store done.
void storeDone();

// check if written before, or settings valid.
bool magicStrPresent(int&);

// load a string.
String loadString(int&);
// load a int.
int loadInt(int&);

// store any struct
void storeStruct(void*, size_t);
void loadStruct(void*, size_t);

#endif