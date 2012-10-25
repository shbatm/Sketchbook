#include <Xmem.h>
//13,11,12
Xmem Xmem(12,11,13);
int addr = 1;
void setup(){
  Serial.begin(115200);
  Xmem.writeXramAddress('a', 1);
  Xmem.writeXramAddress('b', 2);
  Xmem.writeXramAddress('c', 3);
  Serial.println(Xmem.readXramAddress(1));
  Serial.println(Xmem.readXramAddress(2));
  Serial.println(Xmem.readXramAddress(3));
}

void loop(){}
