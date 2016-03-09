//organ_panel_GG_midi.ino

/*
    Quick note button 27 is broken.
*/

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define CHANNEL 1

/* http://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format*/
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(X)  (X & 0x8000 ? 1 : 0),(X & 0x4000 ? 1 : 0), (X & 0x2000 ? 1 : 0), (X & 0x1000 ? 1 : 0), (X & 0x0800 ? 1 : 0), (X & 0x0400 ? 1 : 0), (X & 0x0200 ? 1 : 0), (X & 0x0100 ? 1 : 0),\
                         (X & 0x80 ? 1 : 0), (X & 0x40 ? 1 : 0), (X & 0x20 ? 1 : 0), (X & 0x10 ? 1 : 0), (X & 0x08 ? 1 : 0), (X & 0x04 ? 1 : 0), (X & 0x02 ? 1 : 0), (X & 0x01 ? 1 : 0)


#define BTBP BYTETOBINARYPATTERN
#define BTB(DATA) BYTETOBINARY(DATA)

int statusled = 13;

// definitions for input pins.
#define IN_REGISTER PINK
#define DIR_IN_REGISTER DDRK
uint8_t inpins[] = {PK0, PK1, PK2, PK3, PK4, PK5, PK6, PK7};
const size_t num_inpins = (sizeof(inpins) / sizeof(inpins[0]));

// definitions for output pins.
#define OUT_REGISTERA PORTF
#define OUT_REGISTERB PORTB
#define DIR_OUT_REGISTERA DDRF
#define DIR_OUT_REGISTERB DDRB
#define SET_OUT_REGISTER(X) OUT_REGISTERA = (X & 0xFF); OUT_REGISTERB = (X >> 7);
#define SET_DIR_REGISTER(X) DIR_OUT_REGISTERA |= (X & 0xFF); DIR_OUT_REGISTERB |= (X >> 7);
uint16_t outpins[] = {PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF7+PB0+1, PF7+PB1+1, PF7+PB2+1, PF7+PB3+1};
const size_t num_outpins = (sizeof(outpins) / sizeof(outpins[0]));

uint16_t pinset[num_inpins * num_outpins];
uint16_t old_pinset[num_inpins * num_outpins];

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

    // OUT_REGISTER = (1 << outpins[row]);
    SET_OUT_REGISTER((1 << outpins[row]));
}

uint8_t readCol()
{
    // mask out what ever the serial bits are doing.
    return IN_REGISTER;
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
        // DIR_OUT_REGISTER |= (1 << outpins[i]);
        SET_DIR_REGISTER((1 << outpins[i]));
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
            "row:%d\toutpins[row]:%d\tout: "BTBP" in: "BTBP,
            row,
            outpins[row],
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


    for(int i = 0; i < (num_inpins * num_outpins); i++)
    {
        old_pinset[i] = pinset[i];
        pinset[i] = 0;
    }
    // this works inefficient but works!
    int index = 0;
    for(uint8_t row = 0; row < num_outpins; row++)
    {
        setRow(row);
        uint8_t data = readCol();
        for(uint8_t bit_index = 0; bit_index < num_inpins; bit_index++)
        {
            delay(10);
            if(data)
            {
                pinset[index] = (1 << row) + bit_index;
            }
            index++;
        }
    }
    for(int i = 0; i < (num_inpins * num_outpins); i++)
    {
        if(pinset[i] != old_pinset[i])
        {
            // printPinStates();
        }
        if(pinset[i] > old_pinset[i])
        {
            Serial.print(i);
            Serial.println(":pressed");
            // MIDI.sendNoteOn(i, 127, CHANNEL);
        }
        if(pinset[i] < old_pinset[i])
        {
            Serial.print(i);
            Serial.println(":released");
            // MIDI.sendNoteOff(i, 0, CHANNEL);
        }
    }
}
