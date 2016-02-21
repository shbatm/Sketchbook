//organ_panel_GG_midi.ino

/*
    Quick note button 27 is broken.
*/

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define CHANNEL 1

/* http://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format*/
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(X)  (X & 0x80 ? 1 : 0), (X & 0x40 ? 1 : 0), (X & 0x20 ? 1 : 0), (X & 0x10 ? 1 : 0), (X & 0x08 ? 1 : 0), (X & 0x04 ? 1 : 0), (X & 0x02 ? 1 : 0), (X & 0x01 ? 1 : 0)

#define BTBP BYTETOBINARYPATTERN
#define BTB(byte) BYTETOBINARY(byte)

int statusled = A1;
int testpin = 12;

// definitions for input pins.
#define IN_REGISTER PIND
#define DIR_IN_REGISTER DDRD
uint8_t inpins[] = {PD2, PD3, PD4, PD5, PD6, PD7};
const size_t num_inpins = (sizeof(inpins) / sizeof(inpins[0]));

// definitions for output pins.
#define OUT_REGISTER PORTB
#define DIR_OUT_REGISTER DDRB
uint8_t outpins[] = {PB0, PB1, PB2, PB3, PB4};
const size_t num_outpins = (sizeof(outpins) / sizeof(outpins[0]));

uint8_t pinset[num_inpins * num_outpins];
uint8_t old_pinset[num_inpins * num_outpins];

void randomProgressBlink()
{
    static uint32_t current;
    static uint32_t previous;
    static uint32_t interval;
    static uint8_t ledstate;
    
    current = millis();

    if((current - previous) >= interval)
    {
        previous = current;
        ledstate = !ledstate;
        interval = random(0, 500);
        digitalWrite(statusled, ledstate);
    }

}

void setRow(uint8_t row)
{
    // check so we know we get the right thing.
    if(row >= num_outpins)
        return;

    OUT_REGISTER = (1 << outpins[row]);
}

uint8_t readCol()
{
    // mask out what ever the serial bits are doing.
    return (IN_REGISTER & ~((1 << 0) | (1 << 1))) >> 2;
}

void setupPins()
{
    int i;
    // set pins to input
    for(i = 0; i < num_inpins; i++)
    {
        DIR_IN_REGISTER &= ~(1 << inpins[i]);
    }

    // set pins to output
    for(i = 0; i < num_outpins; i++)
    {
        DIR_OUT_REGISTER |= (1 << outpins[i]);
    }
}


static char printbuffer[100];
void printAnalog()
{
    sprintf(printbuffer,
            "A0: %d | A2: %d | A5: %d | A4: %d\n",
            analogRead(A0),
            analogRead(A2),
            analogRead(A5),
            analogRead(A4));
    Serial.print(printbuffer);
}

void printPinStates()
{
    for(uint8_t row = 0; row < num_outpins; row++)
    {
        setRow(row);
        sprintf(printbuffer,
            "out: "BTBP" in: "BTBP,
            BTB(1 << row),
            BTB(readCol())
            );
        Serial.println(printbuffer);
    }
    Serial.println();
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(statusled, OUTPUT);
    setupPins();
    MIDI.begin(CHANNEL);
    Serial.begin(115200);
}

void loop()
{
    randomProgressBlink();
    if(Serial.available())
    {
        while(Serial.available()) Serial.read();
        printAnalog();
        printPinStates();
    }


    // this works inefficient but works!
    int index = 0;
    for(uint8_t row = 0; row < num_outpins; row++)
    {
        setRow(row);
        // this delay is ugly but it's for the slide contact bounce/grind
        delay(20);
        uint8_t data = readCol();
        for(uint8_t bit_index = 0; bit_index < num_inpins; bit_index++)
        {
            if(data & (1 << bit_index))
            {
                pinset[index] = (1 << bit_index);
            }
            index++;
        }
    }
    for(int i = 0; i < (num_inpins * num_outpins); i++)
    {
        if(pinset[i] > old_pinset[i])
        {
            // Serial.print(i);
            // Serial.println(":pressed");
            MIDI.sendNoteOn(i, 127, CHANNEL);
        }
        if(pinset[i] < old_pinset[i])
        {
            // Serial.print(i);
            // Serial.println(":released");
            MIDI.sendNoteOff(i, 0, CHANNEL);
        }
    }
    for(int i = 0; i < (num_inpins * num_outpins); i++)
    {
        old_pinset[i] = pinset[i];
        pinset[i] = 0;
    }
}
