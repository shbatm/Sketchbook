#include "lightpaper.h"

unsigned long marquee_current = 0;
unsigned long marquee_previous = 0;
uint8_t marquee_interval = 100;
int marquee_count = 0;

const uint8_t screen_width = 80;
const uint8_t screen_height = 8;

const uint8_t segment_width = 5;
const uint8_t letterwidth = 5;
const uint8_t numletters = 16;
const uint8_t buffersize = letterwidth*numletters;
uint8_t buffer[buffersize];

// string buffer.
char string[numletters] = "";
uint8_t stringlength = numletters;

const uint8_t max_letters_visible = screen_width/5;

void writeLPBuffer(uint8_t inbuff, int size)
{
    for(int i = 0;i<buffersize;i++)
    {
        buffer[i] = inbuff;
    }
}

const uint8_t getLPBufferSize()
{
    return buffersize;
}

uint8_t getLPMaxLetterWidth()
{
    return max_letters_visible;
}

uint8_t getLPWidth()
{
    return screen_width;
}

uint8_t getLPHeight()
{
    return screen_height;
}

char *getLPStrBuffer()
{
    return string;
}

uint8_t *getLPBuffer()
{
    return buffer;
}

uint8_t getLPNumLetters()
{
    return numletters;
}

void clearLPStrBuffer()
{
    for(int i = 0;i<numletters;i++)
    {
        string[i] = '\0';
    }
}

void setupRows()
{
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
}

void setupClock()
{
    pinMode(SCLK, OUTPUT);
}

void setupData()
{
    pinMode(R, OUTPUT);
}

void setupLightPaper()
{
    setupRows();
    setupClock();
    setupData();
}

void flashrow(uint8_t row)
{
    if(row < 0 or row > 0x07)
        return;

    PORTD |= ((row & 0x01) << PORTD6);
    PORTC |= ((row & 0x02) >> 1) << PORTC0;
    PORTC |= ((row & 0x04) >> 2) << PORTC1;
    delayMicroseconds(35);
    PORTD &= ~(1 << PORTD6);
    PORTC &= ~(1 << PORTC0);
    PORTC &= ~(1 << PORTC1);
}

void showrow(uint8_t row, uint8_t show)
{
    if(row < 0 or row > 0x07)
        return;

    if(show)
    {
        PORTD |= ((row & 0x01) << PORTD6);
        PORTC |= ((row & 0x02) >> 1) << PORTC0;
        PORTC |= ((row & 0x04) >> 2) << PORTC1;
    }
    else
    {
        PORTD &= ~(1 << PORTD6);
        PORTC &= ~(1 << PORTC0);
        PORTC &= ~(1 << PORTC1);
    }
}

void clock()
{
    PORTD |= (1<<PORTD2);
    PORTD &= ~(1<<PORTD2);
    // digitalWrite(SCLK, LOW);
    // digitalWrite(SCLK, HIGH);
}

void clearRow()
{
    for(int i = 0; i < screen_width; i++)
    {
        PORTD &= ~(1<<PORTD4);
        //digitalWrite(R, LOW);
        clock();
    }
}

void setCol(uint8_t col)
{
    PORTD |= (1<<PORTD4);
    clock();
    PORTD &= ~(1<<PORTD4);
    for(int i = 0;i < col; i++)
    {
        clock();
    }
}


void writeCol(uint8_t data, uint8_t col)
{
    setCol(col);
    setRowData(data);
}

void setRowData(uint8_t data)
{
    for(int i = 0;i < 0x07;i++)
    {
        if((data >> i)&0x01)
        {
            flashrow(i+1);
        }
    }
}

void putStr(char *str)
{
    uint8_t length = strlen(str);
    for(int i = 0; i < length; i++)
    {
        putChar(str[i], i*5);
    }
}


// found on stack overflow.
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void putChar(char character, uint8_t offset)
{
    for(int i = 0; i < 5; i++)
    {
        buffer[i+offset] = reverse(font[character - ' '][i])>>1;
    }
}

void putChar(char character)
{
    for(int i = 0 ; i < 5; i++)
    {
        buffer[i] = font[character-' '][i];
    }
}

void writeout(uint8_t offset)
{
    PORTD |= (1<<PORTD4);
    clock();
    PORTD &= ~(1<<PORTD4);
    for(int i = 0; i< screen_width; i++)
    {
        if(i < buffersize-offset)
        {
            setRowData(buffer[i+offset]);
        }
        clock();
    }
}

void writeout()
{
    PORTD |= (1<<PORTD4);
    clock();
    PORTD &= ~(1<<PORTD4);
    for(int i = 0; i< screen_width; i++)
    {
        setRowData(buffer[i]);
        clock();
    }
}

void setLPBuffer(uint8_t data)
{
    for(int i = 0; i < buffersize; i++)
    {
        buffer[i] = data;
    }
}

void clearLPBuffer()
{
    setLPBuffer(0x00);
}

void marquee(char *str)
{
    marquee_current = millis();
    if(marquee_current - marquee_previous >= marquee_interval)
    {
        marquee_previous = marquee_current;
        marquee_count++;
        stringlength = strlen(str);
        if(marquee_count > (stringlength*letterwidth))
        {
            marquee_count = 0;
        }
    }
    writeout(marquee_count);
}

void showuptime()
{
    stringlength = sprintf(string, "%lu", millis());
    putStr(string);
    writeout();
}
