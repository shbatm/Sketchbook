#ifndef __CMD_H__
#define __CMD_H__

#include <Arduino.h>
#include <SD.h>

typedef struct {
    String name;
    void (*handle)(int, String*);
} command_t;

int getNumCommands();
void setNumCommands(size_t);

// reads input from serial.
String getInput();
// interpreters the input.
void interpretInput(String);
// debug print arguments to a funciton/command.
void printArguments(int, String*);

void printFile(File);
void printDirectory(File, int);

// list info on given dir or file on the sd card..
void listSd(int, String*);
// set various things.
void set(int, String*);
// set current ssid and pass for station mode.
void setSTA(int, String*);

void writeFile(int, String*);
void print3d(int, String*);

// cat the contents of a file to serial.
void cat(int, String*);
// make a directory on the sd card.
void mkdir(int, String*);
// remove a directory or a file on the sd card.
void rm(int, String*);
// move a directory/file on the sd card.
void mv(int, String*);
// create a file or directory on the sd card.
void touch(int, String*);
// restarts esp after X amount of secconds.
void restart(int, String*);
// this prints the available commands
void print_commands(int, String*);
// echo's everything you type back into the command prompt.
void echo(int, String*);


#endif