extern "C" 
{
    #include "user_interface.h"
}

// does not work when you start at 64 !
#define RTCMEM_START (65)
#define SETTINGS_OFFSET (sizeof(uint32_t) * 2)

// store magic number representation.
uint32_t magic = 0xDEADBEEF;
uint32_t magic_read;
uint32_t csum = 0;

typedef struct {
    char aname[16];
    char sname[16];
} test_t __attribute__ ((packed, aligned(4)));

void setup()
{
    Serial.begin(115200);
    Serial.println();

    system_rtc_mem_read(RTCMEM_START, &magic_read, sizeof(uint32_t));
    if(magic_read != magic)
    {
        // magis is not valid so store default and magic.
        system_rtc_mem_write(RTCMEM_START, &magic, sizeof(uint32_t));
        system_rtc_mem_write(RTCMEM_START, &csum, sizeof(uint32_t));
    }
    else
    {
        // magic is valid
    }
    // system_rtc_mem_read(RTCMEM_START, &test1, sizeof(test));
}

void loop(){}