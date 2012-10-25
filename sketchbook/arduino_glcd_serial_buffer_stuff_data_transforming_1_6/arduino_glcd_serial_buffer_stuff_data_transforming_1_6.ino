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
      lcd.print((int)stack[i]);
      i++;
    }
  }
}

int parse(char* buffer){
  /*set up the variables needed*/
  int index = 0;
  int num = 0;
  /*loop aslong as there is stuff in the buffer.*/
  while(buffer[index]!='\0'){
    /*if the first thing we get is a number (must be lol) then put that on the stack*/
    if(isdigit(buffer[index])){
      num = atof(&buffer[index]);
      //num = strtod(buffer,NULL);
      Serial.print("buffer: ");
      Serial.println(buffer);
      Serial.print("num at strtod: ");
      Serial.println(num);
      push(num);
      /*this while loop is there for the way atol(); works on my buffer ..
      in all following this while will be in the same place ... */
      while(isdigit(buffer[index])){
        index++;
      }
    }
    /*else if we get the divider operator, pop() numpop (retrive from stack) then
    divide numpop/num*/
    else if(buffer[index]=='/'){
      /*the first while loop is there to loop till it finds the next number ... :)*/
      while(!isdigit(buffer[index])){
        index++;
      }
      if(isdigit(buffer[index])){
        num = atof(&buffer[index]);
        push(pop()/num);
        while(isdigit(buffer[index])){
          index++;
        }
      }
    }
    /*simular design to the one above, see the diffrence :)?*/
    else if(buffer[index]=='*'){
      while(!isdigit(buffer[index])){
        index++;
      }
      if(isdigit(buffer[index])){
        num = atof(&buffer[index]);
        push(num*pop());
        while(isdigit(buffer[index])){
          index++;
        }
      }
    }
    else{
      index++;
    }
  }
}

void push(uint8_t item){
  Serial.println("stackSize: ");
  Serial.println(stackSize);
  Serial.println("STACKSIZE");
  Serial.println(STACKSIZE);
  /*check if there still is place on the stack to put something on.*/
  if(stackSize<STACKSIZE){
    Serial.println("item: ");
    Serial.println(item,DEC);
    /*if there is place for it put it on there and advance stack point plus one*/
    stack[stackSize++]=item;
  }
  else{
    /*print the informational message that something might be going wrong.*/
    Serial.println("Stack under or overflow");
  }
}

char pop(){
  /*check if the size is not bigger then StackSize*/
  if(stackSize < STACKSIZE){
    /*if it is not then we just go back one place in the stack and return that. and decrease the stack
    pointer by one.*/
    uint8_t item = stack[--stackSize];
    return item;
  }
  else{
    /*print the informational message that something might be going wrong.*/
    Serial.print("stack underflow 0.o...");
    Serial.println("or stack overflow");
  }
}

int pollSerialData(char* buffer,int Size){
  /*look if data on serial line*/
  if(Serial.available()){
    /*if so set the bufferIndex to zero (buffer start)*/
    buffIndex = 0;
    /*then while recieving data, loop till we got nothing left.*/
    while(Serial.available()>0){
      /*for every byte we get we put it in inbyte*/
      char inbyte = Serial.read();
      Serial.print("inbyte: ");
      Serial.println(inbyte);
      /*here we check if buffer is full or not*/
      if(buffIndex == Size){
        /*if it is then we check whether the last byte is printable or not*/
        if(!isprint(inbyte)){
          /*if it is we set it to 0x06, for no perticular reason.*/
          inbyte = 0x06;
        }
        /*then set buffIndex to zero, so that it is at the start of the buffer again.*/
        buffIndex = 0;
        Serial.println("went over the buffer");
        /*clear the buffer of it's data.*/
        memset(buffer,(char)0,Size);
        Serial.print("inbyte at buffer: ");
        Serial.println(inbyte);
        /*make the first place the last byte we recieved*/
        buffer[buffIndex] = inbyte;
        Serial.print("data in buffer: ");
        Serial.println(buffer[0]);
      }
      /*here i check if the byte is printable, if its not we just pass, and don't put it in the buffer.*/
      else if(isprint(inbyte)){
        /*if it is printable, the we put it in the buffer followed by a 0 (not ascii) but a zero byte.*/
        buffer[buffIndex++]=inbyte;
        buffer[buffIndex]= '\0';
      }
    }
    /*when done reading return a 1*/
    return 1; 
    }else{
      /*else return a zero (meening no data on the lines)*/
      return 0;
    }
}
