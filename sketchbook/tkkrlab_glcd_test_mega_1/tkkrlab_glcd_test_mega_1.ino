#define SETENABLE   (PORTF |= 0x01) //enable signal pin.
#define CLEARENABLE (PORTF &= ~0x01)
#define SETRS       (PORTF |= 0x02) //if you set RS you write to data register, else to instruction register.
#define CLEARRS     (PORTF &= ~0x02)
#define SETRW       (PORTF |= 0x04)
#define CLEARRW     (PORTF &= ~0x04)

#define BUSYFLAG 128

void setup(){
  Serial.begin(9600);
  while(checkbusy());
  DDRA = 0xFF;
  DDRF = 0x04;
  
  CLEARRW;
  CLEARRS;
  
  SETENABLE;
  PORTA = 175;
  CLEARENABLE;
  while(checkbusy());
  
  SETENABLE;
  PORTA = 64;
  CLEARENABLE;
  while(checkbusy());
  
  SETENABLE;
  PORTA = 176;
  CLEARENABLE;
  while(checkbusy());
  
  SETENABLE;
  PORTA = 16;
  CLEARENABLE;
  while(checkbusy());
  
  SETENABLE;
  PORTA = 0;
  CLEARENABLE;
  while(checkbusy());
  
  SETENABLE;
  PORTA = 165;
  CLEARENABLE;
  while(checkbusy());
  
  SETENABLE;
  PORTA = 99;
  CLEARENABLE;
  while(checkbusy());
  
  for(int i=0;i<32;i++){
    SETENABLE;
    PORTA = 128|i;
    CLEARENABLE;
    while(checkbusy());
    delay(10);
  }
  SETENABLE;
  PORTA = 172;
  CLEARENABLE;
  while(checkbusy());
  SETENABLE;
  PORTA = 167;
  CLEARENABLE;
  while(checkbusy());
  
  Serial.println("got here");
}

boolean checkbusy(){
  //DDRA = 0x00;
  //SETRW;
  //Serial.println(PINA&BUSYFLAG);
  //CLEARRW;
  //DDRA = 0xFF;
  DDRA = 0x00;
  SETRW;
  int busy = PINA&BUSYFLAG;
  CLEARRW;
  DDRA = 0xFF;
  if(busy){
    return 1;
  }else{
    return 0;
  }
}

void loop(){
  
}
