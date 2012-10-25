#include <TrueRandom.h>
#include <TVout.h>
#include <fontALL.h>

TVout TV;

int Position=0;
int preCheck = 0;

//number of cells needed around to survive.
#define surviveAbility 2
//number around needed for a dead cell to come alife.
#define reproductiveNumber 3
//normaly fieldWidth 31 and field Heigth 12 (these are max value's).
#define fieldWidth 20
#define fieldHeigth 12
#define fieldSize fieldWidth*fieldHeigth
char field[fieldSize];


unsigned long previousTimer = 0;
unsigned long currentTimer = 0;
int interval = 1000;

void fillField(int Min, int Max){
  for(int i=0;i<fieldSize;i++){
    field[i]=TrueRandom.random(Min,Max);
  }
}

void printFieldOnce(){
  TV.set_cursor(0,0);
  for(int i=0;i<fieldSize;i++){
    if(field[i]){
      TV.print("0");
    }
    else{
      TV.print(" ");
    }
  }
}

void printField(int Postion){
  if(Postion%fieldWidth==0){
	TV.println("");
  }
  else if(field[Postion]){
    TV.print("0");
  }
  else{
    TV.print(" ");
  }
}

int checkField(){
  int cellCount = 0;
  for(int i=0;i<fieldSize;i++){
    if(field[i]){
      cellCount++;
    }
  }
  return cellCount;
}

int checkOneBits(){
  int onebits = 0;
  for(int i=0;i<fieldSize;i++){
    onebits |= field[i];
  }
  return onebits;
}
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
  if(Position-fieldWidth+1>0){
    if(field[Position-fieldWidth+1]){
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
  if(Position-fieldWidth-1>0){
    if(field[Position-fieldWidth-1]){
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
  if(Position+fieldWidth-1<fieldSize){
    if(field[Position+fieldWidth-1]){
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
  if(Position+fieldWidth+1<fieldSize){
    if(field[Position+fieldWidth+1]){
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

int totalAround(){
  int around = checkUpper()+checkLower()+checkLeft()+checkRight()+checkUpperLeft()+checkUpperRight()+checkLowerLeft()+checkLowerRight();
  return around;
}

void setup(){
  TV.begin(_PAL);
  TV.select_font(font4x6);
  fillField(0,2);
}

void loop(){
  currentTimer = millis();
  if(currentTimer - previousTimer > interval){
    previousTimer = currentTimer;
    if(checkField() == preCheck){
      fillField(0,2);
    }
    else{
      preCheck = checkField();
    }
    if(!checkOneBits()){
      fillField(0,2);
    }
  }
  else{
    if(field[Position]){
      if(totalAround()==surviveAbility){
        field[Position]=1;
      }
      else if(totalAround()==surviveAbility+1){
        field[Position]=1;
      }
      else{
        field[Position]=0;
      }
    }
    else if(totalAround()==reproductiveNumber){
      field[Position]=1;
    }
    printField(Position);
    Position++;
    if(Position==fieldSize){
      Position = 0;
      TV.set_cursor(0,0);
    }
  }
}
