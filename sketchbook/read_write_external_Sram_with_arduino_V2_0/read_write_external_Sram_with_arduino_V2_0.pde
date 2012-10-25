#include <Xmem.h>

int previous = 0;
Xmem Xmem(10,12,11,A5,A4);
void setup(){
  Serial.begin(115200);
  for(unsigned int addr=0;addr<32768;addr++){
    Xmem.writeXramAddress('P', addr);
  }
  Serial.println("writing ram done: ");
  Serial.println(previous=millis());
  for(unsigned int addr=0;addr<32768;addr++){
    if(addr%20==0){
      Serial.println(Xmem.readXramAddress(addr));
    }
    else{
      Serial.print(Xmem.readXramAddress(addr));
    }
  }
  Serial.println("reading ram done: ");
  Serial.println(millis()-previous);
}

void loop(){}
