#include <Xmem.h>
#include <random.h>
 
int previous = 0;
//latch clock data;
Xmem Xmem(10,12,11);
#define RAMSIZE 32768
void setup(){
  Serial.begin(115200);
  for(unsigned int addr=0;addr<RAMSIZE;addr+=4){
    Xmem.write('A', addr);
    Xmem.write('B', addr+1);
    Xmem.write('C', addr+2);
    Xmem.write('D', addr+3);
  }
  Serial.println("writing ram done: ");
  Serial.println(previous=millis());
  for(unsigned int addr=0;addr<RAMSIZE;addr++){
    if(addr%80==0){
      Serial.println((char)Xmem.read(addr));
    }
    else{
      Serial.print((char)Xmem.read(addr));
    }
  }
  Serial.println("");
  Serial.println("reading ram done: ");
  Serial.println(millis()-previous);
}
 
void loop(){}
