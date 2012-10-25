//setup shift register stuff here.
const int latchpin = 10;
const int clockpin = 12;
const int datapin = 11;
unsigned int addr = 0;

//setup ram pins.
const int writeRamEnable = A1;
const int readRamEnable = A0;

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
  DDRD = B00000010;
  /*
  Here i need to mask also the higher 6 bits.
  it was DDRB = DDRB | B00000000;
  since i need the two lower bits something like:
  if DDRB = B01010111; &   last two bits need to be input thus zero's
            B11111100;
            B01010111;3
         
  */
  DDRB = DDRB&0xFF;
  data = ((PORTD & 0xFC)>>2) | ((PORTB & 0x03)<<6);
//  for(int i=2;i<10;i++){
//    //pinMode(i, INPUT);
//    if(digitalRead(i)){
//      data |= (1<<i-2);
//    }
//    else{
//      data |= (0<<i-2);
//    }
//  }
  //and make readRamEnable HIGH, as to we are done with reading.
  digitalWrite(readRamEnable, HIGH);
  //and return our data.
  return data;
}

void writeXram(byte Data, int addr){
  digitalWrite(writeRamEnable, LOW);
  writeOut(addr);
  DDRD = DDRD | B11111110;
  /*
  uhmm i need last two to be output so like:
  B00000011;
  and thus need to mask the higher 6 bits.
  1's need to stay one's and zero's zero's.
  if DDRB = B01101000; | (or)
            B00000011;
            B01101011;
  
  is what i think.
  
  Thus something like DDRB = DDRB&0xFF;
  
  //
  DDRB = DDRB | B00000011;
  */
  DDRB = DDRB|0x03;
  PORTD = Data<<2;
  PORTB = Data>>6;
//  for(int i=2;i<10;i++){
//    //pinMode(i, OUTPUT);
//    if(Data&1<<i-2){
//      digitalWrite(i, HIGH);
//    }
//    else if(Data^1<<i-2){
//      digitalWrite(i, LOW);
//    }
//  }
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
  //writeXram('h', 0);
  for(unsigned int addr=0;addr<32768;addr++){
    writeXram('P',addr);
  }
  Serial.println(" write ram done: ");
  unsigned int prevCount = millis();
  Serial.println(prevCount);
  for(unsigned int addr=0;addr<32768;addr++){
    if(addr%20==0){
      Serial.println(readXram(addr));
    }
    else{
      Serial.print(readXram(addr));
    }
  }
  Serial.println("");
  Serial.println("millis:");
  Serial.print(millis()-prevCount);
}

void loop(){
}
