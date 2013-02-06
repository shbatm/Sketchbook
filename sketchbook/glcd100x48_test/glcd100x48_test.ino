#define RESET 0xE2
#define ON_OFF 0xAF
#define INITIAL_LINE_ADDRESS 0x40
#define INITIAL_PAGE_ADDRESS 0xB0

int lcdA0 = 12;
int lcdRW = 11;
int lcdEnable = 10;
int lcdDataPins[8] = {9,8,7,6,5,4,3,2};

void setup(){
  pinMode(13,OUTPUT);
  writecommand(RESET);
  delay(1000);
  writecommand(ON_OFF);
  delay(1000);
  writecommand(INITIAL_LINE_ADDRESS);
  delay(1000);
  writecommand(INITIAL_PAGE_ADDRESS);
}
int value = 1;
int bias = 1;
void loop(){
  value = !value;
  digitalWrite(13,value);
  delay(10);
}

void writecommand(byte data){
  digitalWrite(lcdRW, LOW);
  digitalWrite(lcdA0, LOW);
  for(int i = 7;i>=0;i--){
    int value = data & 0x1;
    digitalWrite(lcdDataPins[i], value);
    data = data >> 1;
  }
  digitalWrite(lcdEnable, HIGH);
  delayMicroseconds(10);
  digitalWrite(lcdEnable,LOW);
  delayMicroseconds(10);
  digitalWrite(lcdEnable, HIGH);
}
