int latchpin = 8;
int clockpin = 12;
int datapin = 11;

int switch1 = 2;
int switch2 = 4;
int potpin = A1;
int potpin2 = A0;
int pval2;
int pval;
int dval;

unsigned int addr = 0;
unsigned int i = 0;

void setup(){
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
  pinMode(switch1, OUTPUT);
  pinMode(switch2, OUTPUT);
  pinMode(potpin, OUTPUT);
  pinMode(potpin2, OUTPUT);
  
  Serial.begin(9600);
}

  void loop(){
    for (int i = 0; addr <= 65536; i++) {
      addr = i;
      writeout();
    }
    
  }
  
  void writeout(){
     for (int y = 0; y < 1000; y++){
    digitalWrite(latchpin, LOW);
    digitalWrite(switch1, LOW);
    delayMicroseconds(250);
    shiftOut(datapin, clockpin, MSBFIRST, (addr >> 8));
    digitalWrite(switch1, HIGH);
    delayMicroseconds(250);
    digitalWrite(latchpin, HIGH);
    delayMicroseconds(250);
    
    digitalWrite(latchpin, LOW);
    digitalWrite(switch2, LOW);
    delayMicroseconds(250);
    shiftOut(datapin, clockpin, MSBFIRST, (addr & 255));
    delayMicroseconds(250);
    digitalWrite(switch2, HIGH);
    digitalWrite(latchpin, HIGH);
    delayMicroseconds(250);
     }
  }
