#include <SED1531.h>
 
SED1531 lcd;
 
int lJ[6] = {0x00,0x22,0x42,0x42,0x42,0x3C};
int lO[6] = {0x00,0x3C,0x42,0x42,0x42,0x3C};
int lE[6] = {0x00,0x7E,0x52,0x52,0x52,0x52};
int lY[6] = {0x00,0x06,0x08,0x70,0x08,0x06};

int BPe[6] = {0x00,0x18,0x24,0x24,0x42,0x42};
int Pe[6] = {0x42,0x42,0x42,0x42,0x42,0x42};
int EPe[6] = {0x42,0x42,0x24,0x24,0x18,0x00};

int BPf[6] = {0x00,0x18,0x3C,0x3C,0x7E,0x7E};
int Pf[6] = {0x7E,0x7E,0x7E,0x7E,0x7E,0x7E};
int EPf[6] = {0x7E,0x7E,0x3C,0x3C,0x18,0x00};

void setup(){
  lcd.begin();
  lcd.setCursor(0);
  lcd.setContrast(16);
  delay(2000);
  
  //Clear
  for(int i = 0; i < 5; i++)
  {
    lcd.setCursor(i);
    for(int j = 0; j < 100; j++)
    {
      lcd.writePixData(0);
    }
  }
  lcd.setCursor(0);
  writeChar(lJ);
  writeChar(lO);
  writeChar(lE);
  writeChar(lY);
  Serial.begin(9800);
}

void writeChar(int* Char)
{
  for(int i=0; i<6; i++)
  {
    lcd.writePixData(Char[i]);
  }
}

void loop(){
  lcd.setCursor(1);
  int f = round((millis()/200));
  
  for(int i = 0; i<f;i++)
  {
    lcd.writePixData(0xFF);
  }
  /*int max = 16;
  for(int i = 0; i < max; i++)
  {
    if(i == 0)
    {
      if(f>i)
      {
        writeChar(BPf);
      }
      else
      {
        writeChar(BPe);
      }
    }
    else if(i == max-1)
    {
      if(f>i)
      {
        writeChar(EPf);
      }
      else
      {
        writeChar(EPe);
      }
    }
    else
    {
      if(f>i)
      {
        writeChar(Pf);
      }
      else
      {
        writeChar(Pe);
      }
    }
  }*/
}
