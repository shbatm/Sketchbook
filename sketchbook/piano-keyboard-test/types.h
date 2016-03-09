#ifndef __TYPES_H__
#define __TYPES_H__
typedef struct
{
    uint8_t index;
    uint16_t id;
    uint16_t state;
    uint16_t old_state;
    uint8_t timeout;
    uint8_t timeout_done;
    uint8_t timeout_set;
    int previous_time;
} Pinstate_t;

Pinstate_t pins[12 * 8];


typedef struct
{
    unsigned long previous;
    unsigned long current;
    uint32_t interval;
} timed_t;

#endif