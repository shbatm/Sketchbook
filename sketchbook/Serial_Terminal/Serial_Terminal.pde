#define BufferSize 100
char inputBuffer[BufferSize+1]={'\0'};
int inputBufferIndex = 0;

//setup for timer with millis.
long T1previous = 0;
long T1interval = 100;

void setup(){
  Serial.begin(115200);
  flushBuffer(inputBuffer);
}

void loop(){
  unsigned long T1current = millis();
  getData();
  if(T1current - T1previous > T1interval){
    T1previous = T1current;
    sendData(inputBuffer);
  }
}
//can't pass down arrays, but can pass down array pointers (thier memory location);
void sendData(char* tempBuffer){
  int tempSize = strlen(tempBuffer);
  for(int tempIndex=0;tempIndex<=tempSize;tempIndex++){
    if(tempBuffer[tempIndex]=='\n'){
      Serial.print('\r\n');
      
    }
    else if(tempBuffer[tempIndex]=='\0'){
      continue;
    }
    else{
      Serial.print(tempBuffer[tempIndex]);
    }
  }
  Serial.println("");
}

int getData(){
  while(Serial.available()){
    if(inputBufferIndex<=BufferSize){
      inputBuffer[inputBufferIndex]=Serial.read();
      inputBufferIndex++;
    }
    else{
      inputBufferIndex = 0;
      break;
    }
    return 1;
  }
  return 0;
}

void flushBuffer(char* tempBuffer){
  for(int i = 0;i<BufferSize;i++){
    tempBuffer[i]='\\';
  }
}
