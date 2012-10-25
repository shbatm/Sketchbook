//setup shift register stuff here.
const int latchpin = 10;
const int clockpin = 12;
const int datapin = 11;
unsigned int addr = 0;

//setup ram pins.
const int writeRamEnable = A5;
const int readRamEnable = A4;

//the i/o pins (data lines) are pins 2 till 9
//active low, so to activate write/read drive corosponding pin low.

//a function to write out the shiftregisters.
void writeOut(uint8_t data){
  digitalWrite(latchpin, LOW);
  shiftOut(datapin, clockpin, MSBFIRST, data);
  digitalWrite(latchpin, HIGH);
}

//functions for writing to and reading from external ram
byte readXram(int _addr){
  //we digitalWrite readRamEnale LOW because we want to read.
  digitalWrite(readRamEnable, LOW);
  byte data=B00000000;
  //set the i/o pins to input we want to read ofcourse.
  //at the same time we read for the pins.
  writeOut(_addr);
  Serial.println("at readxram: ");
  for(int i=2;i<10;i++){
    pinMode(i, INPUT);
    if(digitalRead(i)){
      data |= (1<<i-2);
      Serial.print("1");
    }
    else{
      data |= (0<<i-2);
      Serial.print("0");
    }
  }
  Serial.println(" ");
  // we give the address.
//  Serial.println("data at readxmem inside");
//  for(int i=0;i<8;i++){
//    pinMode(i+2, INPUT);
//    if(digitalRead(i+2)){
//      data |= 1<<i;
//      Serial.print("1");
//    }
//    else{
//      data |= 0<<i;
//      Serial.print("0");
//    }
//  }
//  Serial.println("");
//  
  //and make readRamEnable HIGH, as to we are done with reading.
  digitalWrite(readRamEnable, HIGH);
  //and return our data.
  Serial.println("the data xramread: ");
  Serial.println(data);
  return data;
}

void writeXram(byte Data, int addr){
  digitalWrite(writeRamEnable, LOW);
  writeOut(addr);
  Serial.println("written to ram: ");
  for(int i=2;i<10;i++){
    pinMode(i, OUTPUT);
    if(Data&1<<i-2){
      digitalWrite(i, HIGH);
      Serial.print("1");
    }
    else if(Data^1<<i-2){
      digitalWrite(i, LOW);
      Serial.print("0");
    }
  }
  Serial.println("");
  Serial.println("Data: ");
  Serial.println(Data);
  digitalWrite(writeRamEnable, HIGH);
}

void setup(){
  //set pins to output that need to be output.
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
  //set up the read/write pins.
  pinMode(writeRamEnable, OUTPUT);
  pinMode(readRamEnable, OUTPUT);
  //setup serial
  Serial.begin(115200);
  //stuff
  writeXram('h', 0);
  writeXram('e', 1);
  writeXram('l', 2);
  writeXram('l', 3);
  writeXram('o', 4);
  Serial.println("done with write ram: ");
  Serial.println(millis());
  readXram(0);
  readXram(1);
  readXram(2);
  readXram(3);
  readXram(4);
  Serial.println(" ");
  Serial.println("Done");
  Serial.print("millis: ");
  Serial.println(millis());
}

void loop(){
}
