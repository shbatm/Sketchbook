/*
  The rules of the Game of life Are:
  1. Each cell with one or no neighbors dies, as if by loneliness.
  2. Each cell with four or more neighbors dies, as if by overpopulation.
  3. Each cel with tow or three neighbors survives.
  4. Each dead cell or empty block with three neighbors becomes populated, as if by reproduction.
 
  We can set SurviveAbility in the code below, this wil set the amount of cells around the cell
  that are needed to survive. if set to 2 (normal surcumstance) then cells with 2 or 3 neighbors
  survives.
 
  we can also set reproductiveNumber in the code below,
  This will set the number of cells needed around a dead cell/block
  to make it alive.
 
  playing around with these numbers is fun, and i recommend it.
 
  ---------------------------------------------------------------------------------
 
  Because of the nature of the my lcd it being 20x4, it will someTime's reach a stable
  form that does not look like anny stable for fromt he real game of life by John Conway.
  to test if this shape is a real stable form, I went to this website:
  http://www.bitstorm.org/gameoflife/
  I've put in the shape's here That are stable on my lcd, and if they end up in a stable form,
  they must be obeying the rules of the game of life.
  This is actually how I tested if my rules were correct.
 
  A little Technical not on the LCD:
  because of the way the boundary checks are performed, and the lcd is addresed
  shapes might look mishaped,
 
  Lcd:
  ______________________________________
  |if start is here                   1|
  |234 then new line starts here after |
  _________________________________________
 
  so shapes that would go of the side boundary's like:
  _____________________________________________________
  |--------------------------------------------------X|
  |X------------------------------------------------X-|
  |-X------------------------------------------------X|
  |X--------------------------------------------------|
  _____________________________________________________
 
  for a shape that looks like   XX
                               X  X
                                XX
 
  This however does not meen that the end of the fourth line is connected to the beginning,
  of the first line, they're not connect. this only works for the last three lines.
  ----------------------------------------------------------------------------------------------
  Stabel shapes(on the lcd) that i've found to be complex structers when put in: http://www.bitstorm.org/gameoflife/
 
  in this case # represents a living cells, and - represents dead cells.
 
  1. ----#####----
     ----#---#----
     ------#------  
     -----###-----
 
  2. ----#######---
     ----#-----#---
     ------#-#-----
     -----##-##----
 
  3. ----##-##-----  
     ---#-#-#------
     -#--#----#-##-
     -###----##-##-
*/
 
#include <TrueRandom.h>
//includeing files and setting up the lcd and TrueRandom.

#include <LiquidCrystal.h>
LiquidCrystal lcd(22,24,26,28,30,32);

/*
/* diffrent lcd library.
/* #include <IOlcd.h>
/* #include <Wire.h>
 
/* IOlcd lcd;
*/
 
//the higher this number less chance of surviving.
//since this number determines what amount of cells next to a cell it takes to stay
//alife. 8 is max
#define surviveAbility 2
//the higher this number, the less chance for reproduction.
//since this number determines how many cells it takes to turn a dead cell alife.
//9 is max
#define reproductiveNumber 3
//the normal numbers for rules, just like John Conway's Game of life, 
//are 2 for surviveAbility.
//and 3 for reproductiveNumber.
 
//the higher this number, the slower the screen will be printed(in effect slow the program
#define delayVal 10
 
//setting up the field.
#define fieldWidth 16
#define fieldHeigth 4
#define fieldSize fieldWidth*fieldHeigth
#define byte_type uint8_t
byte_type field[fieldSize+1]={0};
 
 
//keeping track of what your checking in a buffer.
int Position = 0;
 
//Timer for checkin once in while, if field is empty.
unsigned long previousTimer = 0;
unsigned long currentTimer = 0;
int interval = 1000;

//for checking field cell count
int prevField = 0;
 
//fill the field with random starting places
void fillField(int Min, int Max){
  int i=0;
  while(i<fieldSize){
    field[i]=TrueRandom.random(Min,Max);
    i++;
  }
}
//these are boundary checks, and also for checking,
//where around a cell a living cell is.
int checkUpper(){
  if(Position-fieldWidth>0){
    if(field[Position-fieldWidth]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkLower(){
  if(Position+fieldWidth<fieldSize){
    if(field[Position+fieldWidth]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkLeft(){
  if(Position-1>0){
    if(field[Position-1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkRight(){
  if(Position+1<fieldSize){
    if(field[Position+1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkUpperLeft(){
  if(Position-(fieldWidth+1)>0){
    if(field[Position-(fieldWidth+1)]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkUpperRight(){
  if(Position-(fieldWidth-1)>0){
    if(field[Position-(fieldWidth-1)]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkLowerLeft(){
  if(Position+(fieldWidth-1)<fieldSize){
    if(field[Position+(fieldWidth-1)]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
 
int checkLowerRight(){
  if(Position+(fieldWidth+1)<fieldSize){
    if(field[Position+(fieldWidth+1)]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
//used to display everything onto the display in the right order, on the right lines.
void displayField(int Position){
  if(field[Position]){
    lcd.write(1);
  }
  else{
    lcd.write((byte)0);
  }
  switch(Position){
    case fieldWidth:
    lcd.setCursor(0,1);
    break;
    case fieldWidth*2:
    lcd.setCursor(0,2);
    break;
    case fieldWidth*3:
    lcd.setCursor(0,3);
    break;
    case fieldWidth*4:
    lcd.setCursor(0,0);
    break;
  }
}
 
//here we create the characters to be displayed, when alive or dead.
byte alife[8] ={
  B01010,
  B01010,
  B10101,
  B10101,
  B01110,
  B01010,
  B01010,
};
 
byte death[8] ={
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
};
 
//here I check how many alive cels there are around a given position in the field.
int totalAround(){
  int around = checkUpper()+checkLower()+checkLeft()+checkRight()+checkUpperLeft()+checkUpperRight()+checkLowerLeft()+checkLowerRight();
  return around;
}
//this is for checking if a field is empty
int checkOneBits(){
  int onebits = 0;
  for(int i=0;i<fieldSize;i++){
    onebits |= field[i];
  }
  return onebits;
}
//this wil check the number of cells on the field.
//if there is the same number for 1 second then refill the field.
int checkField(){
  int cells = 0;
  for(int i = 0;i<fieldSize;i++){
    if(field[i]){
      cells+=field[i];
    }
  }
  return cells;
}
//this is for displaying the field fully once, needed for when the lcd is empty, but you just
//filled the field with random 1's and 0's.
void displayFieldOnce(){
  lcd.home();
  for(int i=0;i<fieldSize;i++){
    displayField(i);
  }
}
 
void setup(){
  delay(1000);
  Serial.begin(115200);
  //setup display workings.
  //diffrent lcd library.
  //lcd.init(0x20,MCP23016);
  lcd.begin(fieldWidth,fieldHeigth);
  //setup costum chars.
  lcd.createChar(0,death);
  lcd.createChar(1,alife);
  lcd.home();
  lcd.clear();
 
 
  fillField(0,2);
  //have to show the field once, or else starting screen empty.
  displayFieldOnce();
}
 
void loop(){
  currentTimer = millis();
  //I look if in the current position is a living cell, if not I check
  //how many cells are around the dead space/blok/cell.
  //basicly we aply the game rules here.
  if(field[Position]){
    if(totalAround()==surviveAbility||totalAround() == surviveAbility+1){
      field[Position]=1;
    }
    /*else if(totalAround()==surviveAbility+1){
      field[Position]=1;
    }*/
    else{
      field[Position]=0;
    }
  }
  else{
    if(totalAround()==reproductiveNumber){
      field[Position]=1;
    }
  }
  // we display the contents of the field.
  displayField(Position);
  Position++;
  if(Position==fieldSize){
    Position=0;
  }
  //once a second I check if the field is empty.
  //this is done this way, to prevent speed los due to 
  //the loop in checkOneBits();
  if(currentTimer - previousTimer > interval){
    previousTimer = currentTimer;
    if(!checkOneBits()){
      fillField(0,2);
      displayFieldOnce();
    }
    if(checkField() == prevField){
      fillField(0,2);
      displayFieldOnce();
    }
    prevField = checkField();
  }
}
