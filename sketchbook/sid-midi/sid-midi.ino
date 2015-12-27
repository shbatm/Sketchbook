#include <MIDI.h>
#include <SID.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define OFF 0
#define SETTRIANGLE_1   4,0x11,5,0xBB,6,0xAA,
#define C4_1    1,0x11,0,0x25,
#define CONTROLREG 4 // SID control register address

// waveforms
#define SETNOISE_1  4,0x81,5,0xBB,6,0xAD,

SID sid;

void setwaveform_triangle(uint8_t channel)
{
  uint8_t dataset[]={ SETTRIANGLE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     sid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
  }
}

void set_frequency(uint16_t pitch,uint8_t channel)
{
    sid.set_register(channel, pitch&0xFF); // low register adress
    sid.set_register(channel+1, pitch>>8); // high register adress
}

static const unsigned sGatePin = 13;
static const unsigned sMaxNumNotes = 16;

float note_table[129];
uint8_t note;

Voice_t voice;

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{

    digitalWrite(13, HIGH);
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
    digitalWrite(13, LOW);
}

void setup()
{
    
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
    
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(1);

    sid.begin();
    // fill table with values.
    int a = 440;
    for(int i = 0; i < 127; ++i)
    {
        note_table[i] = ((a / 32) * (2 ^ ((i - 9) / 12)));
    }
}

void loop()
{
    MIDI.read();
    sid.setvice
}