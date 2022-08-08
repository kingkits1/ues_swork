//File: key.h
#ifndef __KEY_H
#define __KEY_H
#define US_KEY_MASK_ALL 0xF
#define FOOT_MAX_KEY_BIT 0x1
#define FOOT_MIN_KEY_BIT 0x2
#define HAND_MAX_KEY_BIT 0x4
#define HAND_MIN_KEY_BIT 0x8
#define US_MAX_KEY_MASK_ALL 0x5
#define US_MIN_KEY_MASK_ALL 0xA
#define ES_KEY_MASK_ALL 0x10
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
} ENUM_ACTIVED_CONTROL;



typedef struct
{
    // 超声刀操作
    uint16_t foot_max: 1;
    uint16_t foot_min: 1;
    uint16_t hand_max: 1;
    uint16_t hand_min: 1;
    uint16_t hand_close: 1;
    // 高频电刀操作 TODO:
    uint16_t elect_action: 1;
    uint16_t unused  : 10;
} ST_KEY_BIT_DEFINES;

typedef union
{
    uint16_t val;
    ST_KEY_BIT_DEFINES bits;
} UNION_KEY_DEFINES;

typedef struct
{
    UNION_KEY_DEFINES key;
    UNION_KEY_DEFINES key_mask;
    uint16_t ues_checked_flag;// = key.val & key_mask.val

    bool us_min_active_flag;// 低能量激发
    bool us_max_active_flag;// 当高低能量同时激发时，响应高能量激发
    bool es_active_flag;
} ST_KEY_DATA;


void set_actived_mask(ENUM_ACTIVED_CONTROL mask);
void init_key_val(void);
void check_key_actions(uint16_t key);
bool is_us_max_actived(void);
bool is_us_min_actived(void);
bool is_es_min_actived(void);
void set_handmaxmin_val(uint16_t hmax, uint16_t hmin);
void set_hand_close_status(bool status);

#endif

// End of key.h

