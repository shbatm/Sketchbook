#include <MIDI.h>
#include "noteList.h"
#include "pitches.h"

MIDI_CREATE_DEFAULT_INSTANCE();

#ifdef ARDUINO_SAM_DUE // Due has no tone function (yet), overriden to prevent build errors.
#define tone(...)
#define noTone(...)
#endif

// This example shows how to make a simple synth out of an Arduino, using the
// tone() function. It also outputs a gate signal for controlling external
// analog synth components (like envelopes).

static const unsigned sGatePin     = 13;
static const unsigned sAudioOutPin = 9;
static const unsigned sMaxNumNotes = 3;
MidiNoteList<sMaxNumNotes> midiNotes;

#define MAXNOTES 3
unsigned char notes[MAXNOTES] = {0};

uint8_t poly_interval = 120;


void setPolyInterval(byte channel, int bend)
{
    poly_interval = ((unsigned int)bend) / 256;
}

bool isInNotes(byte note)
{
    for(int i = 0; i < MAXNOTES; i++)
    {
        if(notes[i] == note)
        {
            return true;
        }
    }
    return false;
}

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
    // tone(sAudioOutPin, sNotePitches[inNote]);
    if(!isInNotes(inNote))
    {
        for(int i = 0; i < MAXNOTES; i++)
        {
            if(!notes[i])
            {
                notes[i] = inNote;

                break;
            }
        }
    }
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
    if(isInNotes(inNote))
    {
        for(int i = 0; i < MAXNOTES; i++)
        {
            if(notes[i] == inNote)
            {
                notes[i] = 0;
                tone(sAudioOutPin, sNotePitches[inNote]);
                break;
            }
        }
    }
    noTone(sAudioOutPin);
}

// -----------------------------------------------------------------------------

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(setPolyInterval);
    MIDI.begin();
}

void poly_play(uint8_t poly_int)
{
    static unsigned long int poly_cur;
    static unsigned long int poly_pre;

    static int poly_i;
    static uint8_t note;
    note = notes[poly_i];
    
    // play only if we have a note.
    if(note)
    {
        tone(sAudioOutPin, sNotePitches[note]);
    }

    // play tones incrementaly based on time past.
    poly_cur = millis();
    if((poly_cur - poly_pre) >= poly_int)
    {
        poly_pre = millis();
        poly_i++;
        if(poly_i == MAXNOTES)
        {
            poly_i = 0;
        }
    }

    // for(int i = 0; i < MAXNOTES; i++)
    // {
    //     char note = notes[i];
    //     if(note)
    //     {
    //         tone(sAudioOutPin, sNotePitches[notes[i]]);
    //         delay(100);
    //     }
    // }
}

void loop()
{
    MIDI.read();
    poly_play(poly_interval);
}
