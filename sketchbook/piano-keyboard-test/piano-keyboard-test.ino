#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// #define ENABLESERIAL true
#define ENABLESERIAL true

uint16_t matrix[12 * 8] = {};
uint16_t matrix_old[12 * 8] = {};

static char printbuff[100];

typedef struct Pinstate
{
    public:
    uint8_t index;
    uint16_t id;
    uint16_t state;
    uint16_t old_state;
    uint8_t timeout;
    uint8_t timeout_done;
    uint8_t timeout_set;
    int previous_time;
};

Pinstate pins[12 * 8];

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
    
    MIDI.begin(1);
    // Serial.begin(115200);

    int index = 0;
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 12; col++)
        {
            Pinstate *pin = &pins[index];
            pin->id = (1 << col) + row;
            pin->index = index;
            pin->state = 0;
            pin->old_state = 0;

            pin->timeout = 50;
            pin->timeout_done = true;
            pin->timeout_set = false;

            pin->previous_time = 0;
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

void debugprint()
{
    // if(ENABLESERIAL)
    // {
    //     // print_binary(midinum, sizeof(midinum));
    //     Serial.print("octave: ");
    //     Serial.print(midinum);
    //     if(midinum < 10)
    //         Serial.print(" ");
    //     if(midinum < 100)
    //         Serial.print(" ");
    //     print_binary(1 << i, 8);
    //     Serial.print(" ");
    //     print_binary(readCol(), 12);
    //     // print_binary(PINF, 8);
    //     // print_binary(PINK & 0x0F, 4);
    //     Serial.println();
    //     delay(10);   
    // }
}

void loop()
{
    // int index = 0;
    // for(int row = 0; row < 8; row++)
    // {
    //     for(int col = 0; col < 8; col++);
    // }

    for(int row = 0; row < 8; row++)
    {
        setRow(row);
        matrix[row] = readCol();
    }
    for(int i = 0; i < 8; i++)
    {
        if(matrix[i] > matrix_old[i])
            MIDI.sendNoteOn(i+48, 127, 1);
            // Serial.println("pressed");
        if(matrix[i] < matrix_old[i])
            MIDI.sendNoteOff(i+48, 127, 1);
            // Serial.println("released");
    }
    for(int i = 0; i < 8; i++)
    {
        matrix_old[i] = matrix[i];
        matrix[i] = 0;
    }
    delay(10);
}
