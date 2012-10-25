#include <Xmem.h>
#include <random.h>

int previous = 0;
//latch clock data;
Xmem Xmem(10,12,11);
#define RAMSIZE 32768
void setup(){
  Serial.begin(115200);
  for(unsigned int addr=0;addr<RAMSIZE;addr+=4){
    Xmem.writeXramAddress('A', addr);
    Xmem.writeXramAddress('B', addr+1);
    Xmem.writeXramAddress('C', addr+2);
    Xmem.writeXramAddress('D', addr+3);
  }
  Serial.println("writing ram done: ");
  Serial.println(previous=millis());
  for(unsigned int addr=0;addr<RAMSIZE;addr++){
    if(addr%80==0){
      Serial.println((char)Xmem.readXramAddress(addr));
    }
    else{
      Serial.print((char)Xmem.readXramAddress(addr));
    }
  }
  Serial.println("");
  Serial.println("reading ram done: ");
  Serial.println(millis()-previous);
}

void loop(){}
