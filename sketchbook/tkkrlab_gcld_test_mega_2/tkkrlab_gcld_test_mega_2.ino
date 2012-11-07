//#define SETRS           (PORTF |= 0x01) //enable signal pin.
//#define CLEARRS         (PORTF &= ~0x01)
//#define SETRW           (PORTF |= 0x02) //if you set RS you write to data register, else to instruction register.
//#define CLEARRW         (PORTF &= ~0x02)
//#define SETENABLE       (PORTF |= 0x04)
//#define CLEARENABLE     (PORTF &= ~0x04)

#define SETRS digitalWrite(A0, HIGH);
#define CLEARRS digitalWrite(A0, LOW);
#define SETRW digitalWrite(A1, HIGH);
#define CLEARRW digitalWrite(A1, LOW);
#define SETENABLE digitalWrite(A2, HIGH);
#define CLEARENABLE digitalWrite(A2, LOW);

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9800);
  DDRA = 0xff;
  DDRF = 0x07;
  CLEARRS;
  CLEARRW;
  CLEARENABLE;
  
  PORTA = 0xAF;
  SETENABLE;
  while(checkwaitbit());
  CLEARENABLE;
  
  for(int i=0;i<255;i++){
    SETRS;
    SETENABLE;
    PORTA = i;
    while(checkwaitbit());
    CLEARRS;
    CLEARENABLE;
  }
  PORTA = 0xA6;
  SETENABLE;
  while(checkwaitbit());
  CLEARENABLE;
  
  PORTA = 0xA7;
  SETENABLE;
  while(checkwaitbit());
  CLEARENABLE;
  
  PORTA = 0xA5;
  SETENABLE;
  while(checkwaitbit());
  CLEARENABLE;
}

boolean checkwaitbit(){
  PORTA = 0;
  DDRA = 0x00;
  SETRW;
  SETENABLE;
  Serial.println(PINA, BIN);
  delay(10);
  CLEARRW;
  CLEARENABLE;
  DDRA = 0xFF;
  if(PINA>=128){
    return 1;
  }else{
    return 0;
  }
}
void loop(){
  
}
