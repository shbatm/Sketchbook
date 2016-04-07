#include <MIDI.h>
#include "types.h"

MIDI_CREATE_DEFAULT_INSTANCE();

// uint16_t matrix[12 * 8] = {};
// uint16_t matrix_old[12 * 8] = {};
#define NUMROWS  8
#define NUMCOLS  12
uint16_t matrix[NUMROWS][NUMCOLS];
uint16_t matrix_old[NUMROWS][NUMCOLS];

#define PIANOKEYSTART 29
#define NUMKEYS 88
#define OVERLAYSIZE 8
#define OVERLAYSTART 44
uint8_t pianokey_map[NUMROWS][NUMCOLS]
{
    {8, 9, 10, 11, 0, 1, 2, 3, 16, 17, 18, 19},
    {20, 21, 22, 23, 12, 13, 14, 15, 28, 29, 30, 31},
    {32, 33, 34, 35, 24, 25, 26, 27, 40, 41, 42, 43},
    {44, 45, 46, 47, 36, 37, 38, 39, 56, 57, 58, 59},

    {48, 49, 50, 51, 64, 65, 66, 67, 68, 69, 70, 71},
    {60, 61, 62, 63, 76, 77, 78, 79, 80, 81, 82, 83},
    {72, 73, 74, 75, 88, 89, 90, 91, 92, 93, 94, 95},
    {84, 85, 86, 87, 0,  0,  0,  0,  0,  0,  0,  0},
};

uint8_t notes[NUMROWS * NUMCOLS];

static char printbuff[100];

timed_t scan_timer
{
    .previous = 0,
    .current = 0,
    .interval = 1000
};

bool check_timed(timed_t *timed)
{
    timed->current = micros();
    if((timed->current - timed->previous) >= timed->interval)
    {
        timed->previous = timed->current;
        return true;
    }
    return false;
}

void print_binary(uint32_t num, size_t len)
{
    for(uint8_t i = 0;i < len;i++)
    {
        if(num & (1 << i))
        {
            Serial.print('1');
        }
        else
        {
            Serial.print('0');
        }
    }
}

void setRow(uint8_t row)
{
    // keep 4 first bits and or in the row.
    if(row < 4)
    {
        PORTB = (1 << row);
        PORTK &= 0x0F;
    }
    else
    {
        PORTK = (PORTK & 0x0F) | (1 << (row));
        PORTB &= 0xF0;
    }
}

uint16_t readCol()
{
    return (PINF << 4) | (PINK & 0x0F);
}

void generate_keymap()
{
    uint8_t mapped;
    uint8_t index = 0;
    for(int row = 0; row < NUMROWS; row++)
    {
        for(int col = 0; col < NUMCOLS; col++)
        {
            if(index >= NUMKEYS)
                return;
            mapped = pianokey_map[row][col];
            if(index >= OVERLAYSTART)
            {
                notes[mapped] = PIANOKEYSTART + index - OVERLAYSIZE;
            }
            else
            {
                notes[mapped] = PIANOKEYSTART + index;
            }
            index++;
        }
    }

}

void handle_keys()
{
    static uint16_t data;
    static uint8_t note;
    static uint8_t index;

    // if(check_timed(&scan_timer))
    // {
        index = 0;
        for(int row = 0; row < NUMROWS; row++)
        {
            setRow(row);
            data = readCol();
            for(int col = 0; col < NUMCOLS; col++)
            {
                // store the new reading;
                matrix[row][col] = data & (1 << col);

                // compare to old reading if new > old then it's pressed.
                if(matrix[row][col] > matrix_old[row][col])
                {
                    note = notes[index];
                    MIDI.sendNoteOn(note, 127, 1);
                }
                // else if we compare and new < old then it's released.
                if(matrix[row][col] < matrix_old[row][col])
                {
                    note = notes[index];
                    MIDI.sendNoteOff(note, 127, 1);
                    // Serial.println("released");
                }

                // store the new read value as old. 
                matrix_old[row][col] = matrix[row][col];
                index++;
            }
            delay(8);
        }
    // }
}

void setup()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    pinMode(A10, INPUT);
    pinMode(A11, INPUT);

    pinMode(A12, OUTPUT);
    pinMode(A13, OUTPUT);
    pinMode(A14, OUTPUT);
    pinMode(A15, OUTPUT);
    pinMode(53,  OUTPUT);
    pinMode(52,  OUTPUT);
    pinMode(51,  OUTPUT);
    pinMode(50,  OUTPUT);

    Serial.begin(115200);
    generate_keymap();
    MIDI.begin(1);
}

void loop()
{
    handle_keys();
}
