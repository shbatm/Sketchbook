#include <Xmem.h>
#include <random.h>

int index = 0;

Xmem Xmem(11,13,12);

void setup(){
  Serial.begin(115200);
  long i = 0;
  int procent = 0;
  Serial.println("Writing: ");
  for(i=0;i<36767;i++){
    Xmem.writeXramAddress(random(0,i),i);
    if(i%400==0){
      procent++;
      Serial.print(" ");
      Serial.print(i*100/36767);
      Serial.print("%");
      if(procent%20==0){
        Serial.println("");
      }
    }
  }
  Serial.println("Done");
  i = 0;
  while(i<36767){
    char data = Xmem.readXramAddress(i);
    if(i%160==0){
      Serial.println(data);
    }
    else{
      Serial.print(data);
    }
    i++;
  }
  while(1);
}

void loop(){
  if(Serial.available()>0){
    while(Serial.available()){
      Xmem.writeXramAddress(Serial.read(),index++);
      Serial.print((char)Xmem.readXramAddress(index-1));
      Xmem.writeXramAddress('\0',index);
    }
  }
}
