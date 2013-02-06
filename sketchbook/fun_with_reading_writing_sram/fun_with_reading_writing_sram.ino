#include <Xmem.h>
#include <TrueRandom.h>

int index = 0;

//latch, clock, data
Xmem Xmem(10,12,11);

void setup(){
  Serial.begin(115200);
  long i = 0;
  int procent = 0;
  Serial.println("Writing: ");
  for(i=0;i<36767;i++){
    Xmem.write('0',i);
    if(i%(36767/100)==0){
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
  delay(2000);
  i = 0;
  while(i<36767){
    char data = Xmem.read(i);
    if(i%100==0){
      Serial.println(char(data));
    }
    else{
      Serial.print(char(data));
    }
    i++;
  }
  while(1);
}

void loop(){
  if(Serial.available()>0){
    while(Serial.available()){
      Xmem.write(Serial.read(),index++);
      Serial.print((char)Xmem.read(index-1));
      Xmem.write('\0',index);
    }
  }
}
