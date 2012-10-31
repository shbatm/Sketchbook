/**

Aansturen van LCD gekocht bij samenkopen (http://samenkopen.net/action_products/69776)

Gebaseerd op bascom script van Roland van Leusden sayang@zonnet.nl 
Zie http://www.mcselec.com/index2.php?option=com_forum&Itemid=59&page=viewtopic&t=10688

Controller: SED1531 
Size: 48 * 100  

Aansluiting
==================================================================================
Van LCD naar arduino
Pin   LCD                                PIN Arduino

1     GND
2     +5V                                 Digital 13 ( is niet noodzakelijk!! ) 
3     Backlight (GND)
4     AO (L - instruction/ H - data)      Digital 12
5     Read/Write (H - Read / L - Writer)  Digital 11
6     Enable Digital                      Digital 10

Data
7    DB 7 MSB                             Digital 9
8    DB 6                                 Digital 8
9    DB 5                                 Digital 7
10   DB 4                                 Digital 6
11   DB 3                                 Digital 5
12   DB 2                                 Digital 4
13   DB 1                                 Digital 3
14   DB 0 LSB                             Digital 2

*/

int lcdA0 = 12;
int lcdRW = 11;
int lcdEnable = 10;

int displayPower = 13;

int lcdDataPins[] = {9,8,7,6,5,4,3,2};

char str[] = "                    ";

// 5x7 Font
byte lcdFonts[][5] = {
{ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 }, 		
{ 0x00 , 0x00 , 0x5F , 0x00 , 0x00 }, 
{ 0x00 , 0x07 , 0x00 , 0x07 , 0x00 }, 
{ 0x14 , 0x7F , 0x14 , 0x7F , 0x14 }, 
{ 0x24 , 0x2A , 0x7F , 0x2A , 0x12 }, 
{ 0x23 , 0x13 , 0x08 , 0x64 , 0x62 }, 
{ 0x36 , 0x49 , 0x55 , 0x22 , 0x50 }, 
{ 0x00 , 0x05 , 0x03 , 0x00 , 0x00 }, 
{ 0x00 , 0x1C , 0x22 , 0x41 , 0x00 }, 
{ 0x00 , 0x41 , 0x22 , 0x1C , 0x00 }, 
{ 0x08 , 0x2A , 0x1C , 0x2A , 0x08 }, 
{ 0x08 , 0x08 , 0x3E , 0x08 , 0x08 }, 
{ 0x00 , 0x50 , 0x30 , 0x00 , 0x00 }, 
{ 0x08 , 0x08 , 0x08 , 0x08 , 0x08 }, 
{ 0x00 , 0x60 , 0x60 , 0x00 , 0x00 }, 
{ 0x20 , 0x10 , 0x08 , 0x04 , 0x02 }, 
{ 0x3E , 0x51 , 0x49 , 0x45 , 0x3E }, 
{ 0x00 , 0x42 , 0x7F , 0x40 , 0x00 }, 
{ 0x42 , 0x61 , 0x51 , 0x49 , 0x46 }, 
{ 0x21 , 0x41 , 0x45 , 0x4B , 0x31 }, 
{ 0x18 , 0x14 , 0x12 , 0x7F , 0x10 }, 
{ 0x27 , 0x45 , 0x45 , 0x45 , 0x39 }, 
{ 0x3C , 0x4A , 0x49 , 0x49 , 0x30 }, 
{ 0x01 , 0x71 , 0x09 , 0x05 , 0x03 }, 
{ 0x36 , 0x49 , 0x49 , 0x49 , 0x36 }, 
{ 0x06 , 0x49 , 0x49 , 0x29 , 0x1E }, 
{ 0x00 , 0x36 , 0x36 , 0x00 , 0x00 }, 
{ 0x00 , 0x56 , 0x36 , 0x00 , 0x00 }, 
{ 0x00 , 0x08 , 0x14 , 0x22 , 0x41 }, 
{ 0x14 , 0x14 , 0x14 , 0x14 , 0x14 }, 
{ 0x41 , 0x22 , 0x14 , 0x08 , 0x00 }, 
{ 0x02 , 0x01 , 0x51 , 0x09 , 0x06 }, 
{ 0x32 , 0x49 , 0x79 , 0x41 , 0x3E }, 
{ 0x7E , 0x11 , 0x11 , 0x11 , 0x7E }, 
{ 0x7F , 0x49 , 0x49 , 0x49 , 0x36 }, 
{ 0x3E , 0x41 , 0x41 , 0x41 , 0x22 }, 
{ 0x7F , 0x41 , 0x41 , 0x22 , 0x1C }, 
{ 0x7F , 0x49 , 0x49 , 0x49 , 0x41 }, 
{ 0x7F , 0x09 , 0x09 , 0x01 , 0x01 }, 
{ 0x3E , 0x41 , 0x41 , 0x51 , 0x32 }, 
{ 0x7F , 0x08 , 0x08 , 0x08 , 0x7F }, 
{ 0x00 , 0x41 , 0x7F , 0x41 , 0x00 }, 
{ 0x20 , 0x40 , 0x41 , 0x3F , 0x01 }, 
{ 0x7F , 0x08 , 0x14 , 0x22 , 0x41 }, 
{ 0x7F , 0x40 , 0x40 , 0x40 , 0x40 }, 
{ 0x7F , 0x02 , 0x04 , 0x02 , 0x7F }, 
{ 0x7F , 0x04 , 0x08 , 0x10 , 0x7F }, 
{ 0x3E , 0x41 , 0x41 , 0x41 , 0x3E }, 
{ 0x7F , 0x09 , 0x09 , 0x09 , 0x06 }, 
{ 0x3E , 0x41 , 0x51 , 0x21 , 0x5E }, 
{ 0x7F , 0x09 , 0x19 , 0x29 , 0x46 }, 
{ 0x46 , 0x49 , 0x49 , 0x49 , 0x31 }, 
{ 0x01 , 0x01 , 0x7F , 0x01 , 0x01 }, 
{ 0x3F , 0x40 , 0x40 , 0x40 , 0x3F }, 
{ 0x1F , 0x20 , 0x40 , 0x20 , 0x1F }, 
{ 0x7F , 0x20 , 0x18 , 0x20 , 0x7F }, 
{ 0x63 , 0x14 , 0x08 , 0x14 , 0x63 }, 
{ 0x03 , 0x04 , 0x78 , 0x04 , 0x03 }, 
{ 0x61 , 0x51 , 0x49 , 0x45 , 0x43 }, 
{ 0x00 , 0x00 , 0x7F , 0x41 , 0x41 }, 
{ 0x02 , 0x04 , 0x08 , 0x10 , 0x20 }, 
{ 0x41 , 0x41 , 0x7F , 0x00 , 0x00 }, 
{ 0x04 , 0x02 , 0x01 , 0x02 , 0x04 }, 
{ 0x40 , 0x40 , 0x40 , 0x40 , 0x40 }, 
{ 0x00 , 0x01 , 0x02 , 0x04 , 0x00 }, 
{ 0x20 , 0x54 , 0x54 , 0x54 , 0x78 }, 
{ 0x7F , 0x48 , 0x44 , 0x44 , 0x38 }, 
{ 0x38 , 0x44 , 0x44 , 0x44 , 0x20 }, 
{ 0x38 , 0x44 , 0x44 , 0x48 , 0x7F }, 
{ 0x38 , 0x54 , 0x54 , 0x54 , 0x18 }, 
{ 0x08 , 0x7E , 0x09 , 0x01 , 0x02 }, 
{ 0x08 , 0x14 , 0x54 , 0x54 , 0x3C }, 
{ 0x7F , 0x08 , 0x04 , 0x04 , 0x78 }, 
{ 0x00 , 0x44 , 0x7D , 0x40 , 0x00 }, 
{ 0x20 , 0x40 , 0x44 , 0x3D , 0x00 }, 
{ 0x00 , 0x7F , 0x10 , 0x28 , 0x44 }, 
{ 0x00 , 0x41 , 0x7F , 0x40 , 0x00 }, 
{ 0x7C , 0x04 , 0x18 , 0x04 , 0x78 }, 
{ 0x7C , 0x08 , 0x04 , 0x04 , 0x78 }, 
{ 0x38 , 0x44 , 0x44 , 0x44 , 0x38 }, 
{ 0x7C , 0x14 , 0x14 , 0x14 , 0x08 }, 
{ 0x08 , 0x14 , 0x14 , 0x18 , 0x7C }, 
{ 0x7C , 0x08 , 0x04 , 0x04 , 0x08 }, 
{ 0x48 , 0x54 , 0x54 , 0x54 , 0x20 }, 
{ 0x04 , 0x3F , 0x44 , 0x40 , 0x20 }, 
{ 0x3C , 0x40 , 0x40 , 0x20 , 0x7C }, 
{ 0x1C , 0x20 , 0x40 , 0x20 , 0x1C }, 
{ 0x3C , 0x40 , 0x30 , 0x40 , 0x3C }, 
{ 0x44 , 0x28 , 0x10 , 0x28 , 0x44 }, 
{ 0x0C , 0x50 , 0x50 , 0x50 , 0x3C }, 
{ 0x44 , 0x64 , 0x54 , 0x4C , 0x44 }, 
{ 0x00 , 0x08 , 0x36 , 0x41 , 0x00 }, 
{ 0x00 , 0x00 , 0x7F , 0x00 , 0x00 }, 
{ 0x00 , 0x41 , 0x36 , 0x08 , 0x00 }, 
{ 0x08 , 0x08 , 0x2A , 0x1C , 0x08 }, 
{ 0x08 , 0x1C , 0x2A , 0x08 , 0x08 }
};

void setup() {
  pinMode(lcdA0, OUTPUT);
  pinMode(lcdRW, OUTPUT);
  pinMode(lcdEnable, OUTPUT);
  
  pinMode(displayPower, OUTPUT);
  
  for (int i = 0; i <= 7; i++) {
    pinMode(lcdDataPins[i], OUTPUT);
  }
  
  digitalWrite(displayPower, LOW);
  delay(2000);
  digitalWrite(displayPower, HIGH);
  delay(3000);
  lcdInit();
  delay(1000);
  
  writeSomeText();
}

void loop() {
  for (int l = 0; l <= 3; l++) {
    for (byte i = 1; i <= 6; i++) {
      setMarker(i, true);
      delay(400);
      setMarker(i, false);
      delay(100);
    }
  }
  
  setRow(6);
  for( byte j = 1; j <= 4; j++) {
    invertDisplay(true);
    delay(400);
    lcdChar("*-*-LCD REVERSED-*-*");
    invertDisplay(false);
    delay(800);
    resetColumnAdress();
    lcdChar("#-#-#-LCD NORMAL-#-#");
    invertDisplay(true);
  }

 
 for (int k = 0; k < 32; k++) {
   writeLCDcontrast(k);
   setLCDcontrast(k);
   delay(250);
 }
 for (int k = 31; k >= 0; k --) {
   writeLCDcontrast(k);
   setLCDcontrast(k);
   delay(250);
 }
 writeLCDcontrast(16);
 setLCDcontrast(16);
}

/*
Constrast is a value between 0 and 31.
*/
void setLCDcontrast(byte constrast) {
  writeCMDdisplay(0x80 + constrast);
}
void writeLCDcontrast(int contrast) {
   sprintf(str," -> Contrast: %d    ",contrast);
   lcdChar(str);
}

/*
  LCD init commands
*/
void lcdInit() {
  
  //the following actions are performed to init the lcd
  writeCMDdisplay(0xe2);                                //reset display by soft
  delay(1000);
  writeCMDdisplay(0xa1);                                //ADC select
  writeCMDdisplay(0xa2);                                //lcd bias 1/8
  writeCMDdisplay(0x2c);                                //power
  writeCMDdisplay(0x2e);                                //power
  writeCMDdisplay(0x2f);                                //power
  writeCMDdisplay(0xa6);                                //normal / reverse
  writeCMDdisplay(0x8f);                                //set electronic control
  writeCMDdisplay(0xa4);                                //display off
  writeCMDdisplay(0xaf);                                //display on
  writeCMDdisplay(0x40);                                //sel dram line 1 for com1

}

void writeSomeText() {
   setRow(1);                                         
   lcdChar( "012345678901234567890");
   setRow(2);
   lcdChar( "ABCDEFGHIJKLMNPQRSTUV");
   setRow(3);
   lcdChar( "wxyzabcdefghijklmnopq");
   setRow(4);
   lcdChar( "  !#$%&'( )@^-");
   setRow(5);
   lcdChar( "  *+-/<>?;:[]");
   setRow(6);
   lcdChar( "* Arduino Rocks ! *");
}



/*  Control de markers boven in het display
 MARKER 1  20  -- pijltjes boven elkaar
 MARKER 2  31  -- 2 horiz. streepjes van barcode
 MARKER 3  32  -- barcode
 MARKER 4  57  -- batterij
 MARKER 5  69  -- sterretje
 MARKER 6  78  -- pijltje UP
*/
void setMarker(byte marker, boolean on) {


  byte highNibble, lowNibble;
  byte markerLCD;

   switch (marker) {
     case 1 : markerLCD = 20;
             break;
     case 2 : markerLCD = 31;
            break;
     case 3: markerLCD = 32;
            break;
     case 4: markerLCD = 57;
            break;
     case 5: markerLCD = 69;
           break;
     case 6: markerLCD = 78;
           break;
   }
 
   lowNibble = markerLCD & 0xF;    // Mask out upper nibble
   highNibble = markerLCD;        
   highNibble = highNibble >> 4;         // Shift upper 4 bits to lower
   bitSet(highNibble, 4);          // Set 5th bit high
   
   writeCMDdisplay(0xb6);          //Set page Address
   writeCMDdisplay(highNibble);     //Set column Address high nibble
   writeCMDdisplay(lowNibble);      //Set column Address low nibble
   
   writeDATAdisplay(on);
}

/*
  Put COMMAND to display (for configuration purpose)
*/
void writeCMDdisplay(byte cmd) {
  digitalWrite(lcdRW, LOW);
  digitalWrite(lcdA0, LOW);
  
  byte data = cmd;
  
  for (int i = 7; i >= 0; i--) {
    int value = data & 0x1;
    digitalWrite(lcdDataPins[i], value);
    data = data >> 1;
  }

  digitalWrite(lcdEnable, HIGH);
  delayMicroseconds(10);
  digitalWrite(lcdEnable, LOW);
  delayMicroseconds(10);
  digitalWrite(lcdEnable, HIGH);
}

/*
  Put DATA to display (pixel data)
*/
void writeDATAdisplay(byte lcdData) {
  digitalWrite(lcdRW, LOW);
  digitalWrite(lcdA0, HIGH);

  byte data = lcdData;
  
  for (int i = 7; i >= 0 ; i--) {
    int value = data & 0x1;
    digitalWrite(lcdDataPins[i], value);
    data = data >> 1;
  }
  
  digitalWrite(lcdEnable, HIGH);
  delayMicroseconds(10);
  digitalWrite(lcdEnable, LOW);
  delayMicroseconds(10);
  digitalWrite(lcdEnable, HIGH);
}


void invertDisplay(boolean reverse) {
  writeCMDdisplay(0xA6 + (reverse ? 0:1));
}



/*
 Select the row where to write the text
 
 row value between 1 and 6
*/
void setRow(byte row) {
  byte page = 0xB0 + (row - 1);
  
  writeCMDdisplay(page);
  writeCMDdisplay(0x08);
  writeCMDdisplay(0x00);
  
  lcdChar("                    ");   //Make page empty
  
  writeCMDdisplay(page);
  writeCMDdisplay(0x08);
  writeCMDdisplay(0x00);
}


/*
  Send text string to display
*/
void lcdChar(String text) {
   int charCount = text.length();
   
   //There is only room for 20 characters
   if (charCount > 20) {
     charCount = 20;      
   }
   
   //Start Read-Modify-Write
   writeCMDdisplay(0xE0);
   
   //Loop trough text
   for (int i = 0; i < charCount; i++) {
     char currentChar = text.charAt(i);
     
     // Character table starts at Ascii-32, so deduct 32 to get it even with the ascii table
     int lcdCharPosition = currentChar - 32;

     for (int j = 0; j < 5; j++) {
       writeDATAdisplay(lcdFonts[lcdCharPosition][j]);
     }
   }
   
   //End Read-Modify-Write
  writeCMDdisplay(0xEE);
  
  resetColumnAdress();

}

/*
 Reset column address to the first position in the page
*/
void resetColumnAdress() {
  writeCMDdisplay(0x10);
  writeCMDdisplay(0x00);
}




