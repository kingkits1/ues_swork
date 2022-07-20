//File: key.h
#ifndef __KEY_H
#define __KEY_H
typedef enum 
{
    EM_ALL_DISABLE,
    // US only
    EM_US_HAND_ONLY,
    EM_US_FOOT_ONLY,
    EM_US_HAND_AND_FOOT,
    // ES only
    EM_ES_ONLY,
    // US+ES
    EM_ES_US_HAND_ONLY,
    EM_ES_US_FOOT_ONLY,
    EM_ES_US_HAND_AND_FOOT
}ENUM_ACTIVED_CONTROL;



typedef struct
{
    // ³¬Éùµ¶²Ù×÷
    uint16_t foot_max:1;
    uint16_t foot_min:1;
    uint16_t hand_mac:1;
    uint16_t hand_min:1;
    // ¸ßÆµµçµ¶²Ù×÷ TODO:
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
}ST_KEY_DATA;
#endif

// End of key.h

