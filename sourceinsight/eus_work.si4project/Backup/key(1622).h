//File: key.h
#ifndef __KEY_H
#define __KEY_H
typedef struct
{
    uint16_t foot_max:1;
    uint16_t foot_min:1;
    uint16_t hand_mac:1;
    uint16_t hand_min:1;
    uint16_t unused  :12;
}ST_KEY_BIT_DEFINES;

typedef union
{
    uint16_t val;
    ST_KEY_BIT_DEFINES bits;
} UNION_KEY_DEFINES;

typedef struct
{
    UNION_KEY_DEFINES key;
    UNION_KEY_DEFINES key_mask;
    uint16_t active_flag;
}
#endif

// End of key.h

