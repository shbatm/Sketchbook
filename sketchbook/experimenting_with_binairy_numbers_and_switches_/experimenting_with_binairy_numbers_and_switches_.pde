int Bi1 = 3;
int Bi2 = 5;
int Bi3 = 7;
int Bi4 = 9;
int Bi5 = 11;
int Bi6 = 13;

int Bit1;
int Bit2;
int Bit3;
int Bit4;
int Bit5;
int Bit6;


void setup(){
  pinMode(Bi1, INPUT);
  pinMode(Bi2, INPUT);
  pinMode(Bi3, INPUT);
  pinMode(Bi4, INPUT);
  pinMode(Bi5, INPUT);
  pinMode(Bi6, INPUT);
  
  Serial.begin(9600);
}

void loop(){
  Bit1 = digitalRead(Bi1);
  Bit2 = digitalRead(Bi2);
  Bit3 = digitalRead(Bi3);
  Bit4 = digitalRead(Bi4);
  Bit5 = digitalRead(Bi5);
  Bit6 = digitalRead(Bi6);
  
  //Serial.println("button's pressed in this order 1 is pressed button.");
  Serial.print(Bit1);
  Serial.print(Bit2);
  Serial.print(Bit3);
  Serial.print(Bit4);
  Serial.print(Bit5);
  Serial.print(Bit6);
  Serial.println(" ");
  
  //delay(500);
  
}
