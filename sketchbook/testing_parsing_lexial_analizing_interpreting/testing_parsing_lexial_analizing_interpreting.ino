#define byte_type uint8_t
#define buffSize 8
byte_type textBuffer[buffSize];
int buffIndex = 0;

/*
trying to make a calculator.

so in math in calculations it's multiplication and divition,
before addition and substraction.

so it think i'll make two stacks,
one which holds numbers, the other holds symbols.

i think it wil be most like: 
http://en.wikipedia.org/wiki/Shunting_yard_algorithm

*/

byte_type operatorStack[buffSize];
int operatorStackCount = 0;
byte_type stack[buffSize];
int stackCount = 0;
int stackIndex = 0;

byte_type operatorTokens[4]={'/','*','+','-'};

int tokCount = 0;
int result = 0;

long T1previous = 0;
long T1interval = 500;

void setup(){
  Serial.begin(115200);
}

void loop(){
  unsigned long T1Current = millis();
  getData(textBuffer);
  if(T1Current - T1previous > T1interval){
    
  parseData(textBuffer);
    T1previous = T1Current;
    Serial.print("textBuffer: ");
    sendData(textBuffer, 0);
    Serial.print("stack: ");
    sendData(stack, 2);
    Serial.print("operatorStack: ");
    sendData(operatorStack, 0);
    Serial.println("");
  }
}

void interpretData(){
  switch(operatorStack[stackIndex]){
    case '*':
    stack[stackIndex]=stack[stackIndex]*stack[stackIndex+1];
    break;
    case '/':
    stack[stackIndex]=stack[stackIndex]/stack[stackIndex+1];
    break;
    case '+':
    stack[stackIndex]=stack[stackIndex]+stack[stackIndex+1];
    break;
    case '-':
    stack[stackIndex]=stack[stackIndex]-stack[stackIndex+1];
    break;
    default:
    stackIndex++;
    break;
  }
}

byte parseData(byte_type* textBuffer){
  if(tokCount==strlen((char*)textBuffer)){
    tokCount=0;
  }
  else if(textBuffer[tokCount]!='\0'){
    if(isdigit(textBuffer[tokCount])){
      stack[stackCount]=(textBuffer[stackCount]-48);
      stack[stackCount+1]='\0';
      stackCount++;
      if(stackCount==strlen((char*)textBuffer)-1){
        stackCount=0;
      }
    }
    if(isdigit(textBuffer[tokCount]==0)){
      operatorStack[operatorStackCount]=textBuffer[operatorStackCount];
      operatorStack[operatorStackCount+1]='\0';
      operatorStackCount++;
      if(operatorStackCount==strlen((char*)textBuffer)-1){
        operatorStackCount=0;
      }
    }
  }
  tokCount++;
}
void sendData(byte_type* tempBuffer, int type){
  int tempSize = strlen((char*)tempBuffer);
  for(int tempIndex=0;tempIndex<=tempSize;tempIndex++){
    if(tempBuffer[tempIndex]=='\n'){
      Serial.write('\r\n');
    }
    else if(tempBuffer[tempIndex]=='\0'){
      break;
    }
    else{
      if(type==0){
        Serial.print((char)tempBuffer[tempIndex]);
        Serial.print(" ");
      }
      else if(type==1){
        Serial.print((int)tempBuffer[tempIndex]);
        Serial.print(" ");
      }
      else if(type==2){
        Serial.print(tempBuffer[tempIndex], HEX);
        Serial.print(" ");
      }
    }
  }
  Serial.println("");
}

int getData(byte_type* inputBuffer){
  if(Serial.available()){
    delayMicroseconds(1);
    while(Serial.available()){
      inputBuffer[buffIndex]=Serial.read();
      inputBuffer[buffIndex+1]='\0';
      buffIndex++;
      if(buffIndex==buffSize-1){
        buffIndex=0;
        break;
      }
    }
    return 1;
  }
  else{
    return 0;
  }
}

int getData1(byte_type* inputBuffer){
  while(Serial.available()){
    inputBuffer[buffIndex]=Serial.read();
    if(buffIndex<buffSize){
      inputBuffer[buffIndex+1]='\0';
    }
    else if(buffIndex==buffSize){
      buffIndex=0;
      break;
    }
    buffIndex++;
  }
  return 0;
}

void emptyBuffer(byte_type* inputbuffer){
  for(int i=0;i<strlen((char*)inputbuffer);i++){
    inputbuffer[i]='\0';
  }
}
