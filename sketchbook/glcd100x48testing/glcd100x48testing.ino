#define SETENABLE     (PORTB &= ~0X10)
#define CLEARENABLE   (PORTB |= 0X10)
#define LCDENABLE     (0x10)
#define SETRW         (PORTB &= ~0X20)
#define CLEARRW       (PORTB |= 0X20)
#define LCDRW         (0x20)
#define SETA0         (PORTB &= ~0X40)
#define CLEARA0       (PORTB |= 0X40)
#define LCDA0         (0x40)
#define SETPOWER      (PORTB &= ~0X80)
#define CLEARPOWER    (PORTB |= 0X80)
#define LCDPOWER      (0X80)

int lcdA0 = 12;
int lcdRW = 11;
int lcdEnable = 10;
int lcdPower = 13;

void setup(){
  Serial.begin(9600);
  
  pinMode(lcdA0, OUTPUT);
  pinMode(lcdRW, OUTPUT);
  pinMode(lcdEnable, OUTPUT);
  pinMode(lcdPower, OUTPUT);
  //set data us to output, put all the data pins on one port easy use.
  DDRA = 0xFF;

  digitalWrite(lcdPower,LOW);
  delay(200);
  digitalWrite(lcdPower, HIGH);
  delay(300);
  
  write_command(0xe2);//reset
  //delay(1000);
  write_command(0x2c);//first electronic mode
  write_command(0x2e);//second electronic mode
  write_command(0x2f);//third electronic mode 
  write_command(0xa2);//set contrast level resolution.
  write_command(0x80+0);//contrast level
  write_command(0xaf);//turn on lcd just in case.
  write_command(0xa1);//adc select wheter to write from left to right or visa versa.
  write_command(0x80|16);//set contrast half way.
}
void loop(){
  for(int i = 0;i<255;i++){
    write_data(i);
    delay(20);
  }
}

byte readBusy(){
  DDRA |= 0x00;
  byte busy = PINA;
  DDRA |= 0xff;
}

void write_data(uint8_t data){
  digitalWrite(lcdRW, LOW);
  digitalWrite(lcdA0, HIGH);
  PORTA = byteShift(data);
  digitalWrite(lcdEnable, HIGH);
  digitalWrite(lcdEnable, LOW);
  digitalWrite(lcdEnable, HIGH);
}

void write_command(uint8_t data){
  digitalWrite(lcdA0,LOW);
  digitalWrite(lcdRW, LOW);
  PORTA = byteShift(data);
  digitalWrite(lcdEnable, HIGH);
  digitalWrite(lcdEnable, LOW);
  digitalWrite(lcdEnable, HIGH);
}

byte byteShift(byte num){
  byte var = 0;
  byte i, x, y, p;
  byte s = 8;

  for(i=0;i<(s/2);i++){
    p = s-i-1;
    x = num&(1<<p);
    x = x >> p;
    y = num & (1<<i);
    y = y>>i;
    var = var | (x<<i);
    var = var | (y<<p);
  }
  return var;
}


