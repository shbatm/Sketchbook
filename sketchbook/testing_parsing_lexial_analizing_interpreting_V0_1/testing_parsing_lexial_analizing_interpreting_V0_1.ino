#define byte_type uint8_t
#define buffSize 10
byte_type textbuffer[buffSize];
int buffindex = 0;

byte_type operatorStack[buffSize];
int operatorStackCount = 0;
byte_type stack[buffSize];
int stackCount = 0;
int stackIndex = 0;

int tokCount = 0;
int tempIndex = 0;

long T1previous = 0;
int T1interval = 500;

void setup(){
  Serial.begin(115200);
}

void loop(){
  unsigned long T1Current = millis();
  if(getData(textbuffer)){
    parseData();
  }
  if(T1Current - T1previous > T1interval){
    T1previous = T1Current;
    Serial.print("textBuffer: ");
    sendData(textbuffer, 1);
    Serial.print("stack: ");
    sendData(stack, 0);
    Serial.print("operatorStack: ");
    sendData(operatorStack, 1);
    Serial.println("");
  }
}

void parseData(){
  //stack[0]='\0';
  //operatorStack[0]='\0';
  for(stackCount=0;stackCount<strlen((char*)textbuffer);stackCount++){
    if(isdigit(textbuffer[stackCount])){
      stack[stackCount] = (textbuffer[stackCount]-48);
      stack[stackCount+1]='\0';
    }
    else{
      operatorStack[operatorStackCount]=textbuffer[stackCount];
      operatorStack[operatorStackCount+1]='\0';
      operatorStackCount++;
    }
  }
  operatorStackCount=0;
}

void sendData(byte_type* tempBuffer_, int type){
  for(int tempIndex_=0;tempIndex_<strlen((char*)tempBuffer_);tempIndex_++){
    if(tempBuffer_[tempIndex_]=='\n'){
      Serial.write('\r\n');
    }
    else if(tempBuffer_[tempIndex_]=='\0'){
      break;
    }
    else{
      if(type == 0){
        Serial.print(tempBuffer_[tempIndex_]);
      }
      else if(type== 1){
        Serial.print((char)tempBuffer_[tempIndex_]);
      }
      else if(type == 2){
        Serial.print(tempBuffer_[tempIndex_], HEX);
      }
    }
  }
  Serial.println("");
}

int getData(byte_type* tempBuffer){
  if(Serial.available()){
    while(Serial.available()){
      tempBuffer[tempIndex]=(byte_type)Serial.read();
      tempBuffer[tempIndex+1]='\0';
      tempIndex++;
      if(tempIndex+1==buffSize){
        tempIndex = 0;
        break;
      }
    }
    return 1;
  }
  else{
    return 0;
  }
}
