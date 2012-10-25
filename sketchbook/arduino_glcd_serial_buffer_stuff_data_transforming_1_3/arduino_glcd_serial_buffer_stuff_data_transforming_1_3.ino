#include <glcd.h>
#include <fonts/allFonts.h>


/*define constants like screen width,height and screen size*/
#define WIDTH 21
#define HEIGHT 8
#define SCREENSIZE 50

#define STACKSIZE 50

/*define a buffer that holds our data, and create a index for it set to 0*/

int buffIndex = 0;
char Buffer[SCREENSIZE+1];
/*create space for a parse tree i think 50 places is enough.*/
char stack[STACKSIZE];
int stackSize = 0;
/*this is for timing purpose, (basicly for displaying serial data once every 5 seconds*/
long previousMillis = 0;
int interval = 1000;
/*create a instance of lcd*/
glcd lcd;

#define OPLISTSIZE 4
char Operator[OPLISTSIZE] = {'*','/','+','-'};


void setup(){
  /*begin serial comunication and set the baud at which to comunicate.*/
  Serial.begin(115200);
  /*make sure the stack is empty!*/
  stack[0]='\0';
  /*initialize the lcd, and set a font of 5x7*/
  lcd.Init();
  lcd.SelectFont(System5x7);
  /*screen size is 8192 and widht is 128 and height is 64*/
}

void loop(){
  /*created a function that polls for serial data, then checks if it got anny
    if it recieved data returns a 1 else a zero this way i can refresh the screen,
    and know when i have new data to work with.*/
  if(pollSerialData(Buffer,SCREENSIZE)){
    parse(Buffer);
    //Serial.print("Stack: ");
    //Serial.println((int)stack);
  }
  /*see if we don't recieve anything i just go and refresh the screen and write, the data to it*/
  else{
    lcd.CursorTo(0,0);
  }
  /*here i keep track of the clock,*/
  unsigned long currentMillis = millis();
  /*then we check if a certain time has passed (interval) if it has we print debugging stuff to the serial*/
  if(currentMillis - previousMillis > interval){
    previousMillis = currentMillis;
    lcd.ClearArea();
    lcd.print("buffer: ");
    lcd.println(Buffer);
    lcd.print("Stack: ");
    int i = 0;
    while(stack[i]!='\0'){
    lcd.print(stack[i], DEC);
    i++;
    }
  }
}

int parse(char* buffer){
  int index = 0;
  long num = 0;
  char backOp;
  while(buffer[index]!='\0'){
    if(isdigit(buffer[index])){
      num = atol(&buffer[index]);
      push(num);
      while(isdigit(buffer[index])){
        index++;
      }
    }
    else if(buffer[index]=='*'){
      //Serial.print("pop():");
      //Serial.println(backOp = pop(), DEC);
      //push(backOp);
      //push(buffer[index]);
      while(!isdigit(buffer[index])){
        index++;
      }
      if(isdigit(buffer[index])){
        num = atol(&buffer[index]);
        push(num*pop());
        while(isdigit(buffer[index])){
          index++;
        }
      }
    }
    //else if(buffer[index]=='/')
  }
}

boolean CheckOperator(char Op){
  for(int i = 0;i<OPLISTSIZE;i++){
    if(Operator[i]==Op){
      Serial.print(Operator[i]);
      return true;
    }
  }
  return false;
}

void push(uint8_t item){
  Serial.println("stackSize: ");
  Serial.println(stackSize);
  Serial.println("STACKSIZE");
  Serial.println(STACKSIZE);
  if(stackSize<STACKSIZE){
    Serial.println("item: ");
    Serial.println(item,DEC);
    stack[stackSize++]=item;
    //stack[stackSize]='\0';
  }
}

char pop(){
  if(stackSize < STACKSIZE){
    char item = stack[--stackSize];
    return item;
  }
  else{
    Serial.print("stack underflow 0.o...");
    Serial.println("or stack overflow");
  }
}

int pollSerialData(char* buffer,int Size){
  if(Serial.available()){
    buffIndex = 0;
    while(Serial.available()>0){
      char inbyte = Serial.read();
      Serial.print("inbyte: ");
      Serial.println(inbyte);
      
      if(buffIndex == Size){
        if(!isprint(inbyte)){
          inbyte = 0x06;
        }
        buffIndex = 0;
        Serial.println("went over the buffer");
        memset(buffer,(char)0,Size);
        Serial.print("inbyte at buffer: ");
        Serial.println(inbyte);
        buffer[buffIndex] = inbyte;
        Serial.print("data in buffer: ");
        Serial.println(buffer[0]);
      }else if(isprint(inbyte)){
        buffer[buffIndex++]=inbyte;
        buffer[buffIndex]= '\0';
      }
    }
    return 1; 
    }else{
      return 0;
    }
}
