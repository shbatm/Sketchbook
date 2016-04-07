//midi-keyboard-duality.ino

// PORTL == OUTPUT
// PORTF == OUTPUT
// PORTK == INPUT

#define NUMROWS 8
#define NUMCOLS 14
#define NUMKEYS 45

uint16_t matrix[NUMROWS][NUMCOLS];
uint16_t matrix_old[NUMROWS][NUMCOLS];

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

void setup_pins()
{
    DDRK = 0xFF;

    DDRL = 0x00;
    DDRL |= (1<<6)|(1<<7);
    DDRF = 0x00;
}

uint16_t setRow(uint8_t row)
{
    PORTK = (1 << row);
    return PORTK;
}

uint16_t readCol()
{
    uint16_t data = (PINF << 8) | PINL ;
    return data;
}

void scan_keys()
{
    uint16_t index = 0;
    for(uint8_t row = 0; row < NUMROWS; row++)
    {
        uint8_t set = setRow(row);
        uint16_t data = readCol();
        for(uint16_t col = 0; col < NUMCOLS; col++)
        {
            matrix[row][col] = data & (1 << col);
            if(matrix[row][col] > matrix_old[row][col])
            {
                Serial.println("pressed");
            }
            if(matrix[row][col] < matrix_old[row][col])
            {
                Serial.println("released");
            }
            matrix_old[row][col] = matrix[row][col];
        }
    }
}

void debug()
{
    for(uint8_t row = 0; row < NUMROWS; row++)
    {
        uint16_t set = setRow(row);
        delay(10);
        uint16_t data = readCol();
        print_binary(set, 8);
        Serial.print(" ");
        print_binary(data, 16);
        Serial.println();
    }
    Serial.println();
    delay(10);

}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    setup_pins();
}

void loop()
{
    debug();
    // scan_keys();
}
