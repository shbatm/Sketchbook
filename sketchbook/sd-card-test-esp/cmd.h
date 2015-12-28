#ifndef __CMD_H__
#define __CMD_H__

#include <Arduino.h>
#include <SD.h>

typedef struct {
    String name;
    void (*handle)(int, String* argv);
} command_t;

int getNumCommands();
void setNumCommands(size_t);

String getInput();
void interpretInput(String);

void printFile(File);
void printDirectory(File, int);

void listSd(int, String*);
void cat(int, String*);


#endif