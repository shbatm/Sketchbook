int latchpin = 8;
int clockpin = 12;
int datapin = 11;

int switch1 = 2;
int switch2 = 4;
int potpin = A1;
int potpin2 = A0;
int pval2;
int pval;
int bval;

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
    bval = analogRead(potpin);
    bval = map(bval, 10, 1023, 100, 1000);
    
    for (int y = 0; y < bval; y++){
    digitalWrite(latchpin, LOW);
    digitalWrite(switch1, HIGH);
    shiftOut(datapin, clockpin, MSBFIRST, (addr >> 8));
    digitalWrite(switch1, LOW);
    digitalWrite(latchpin, HIGH);
    delayMicroseconds(500);
    digitalWrite(latchpin, LOW);
    digitalWrite(switch2, HIGH);
    shiftOut(datapin, clockpin, MSBFIRST, (addr & 255));
    digitalWrite(switch2, LOW);
    digitalWrite(latchpin, HIGH);
    delayMicroseconds(500);
     }
  }
 
