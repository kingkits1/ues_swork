// File:key.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "key.h"
static ST_KEY_DATA key_val;
static uint16_t key;

void get_key_status(void)
{
    // Foot max 激发时为低电平
    if(PBin(14) == 0) key = FOOT_MAX_KEY_BIT;
    else key = 0;
    // Foot min
    if(PJin(2) == 0) key |= FOOT_MIN_KEY_BIT;
    // HAND MAX

}
void set_actived_mask(ENUM_ACTIVED_CONTROL mask)
{
    switch(mask)
    {
    case EM_ALL_DISABLE:
        key_val.key_mask.val = 0;
        break;
    // US only
    case EM_US_HAND_ONLY:
        key_val.key_mask.val = 0xc;
        break;
    case EM_US_FOOT_ONLY:
        key_val.key_mask.val = 0x3;
        break;
    case EM_US_HAND_AND_FOOT:
        key_val.key_mask.val = 0xf;
        break;
    // ES only
    case EM_ES_ONLY:
        key_val.key_mask.val = 0x10;
        break;
    // US+ES
    case EM_ES_US_HAND_ONLY:
        key_val.key_mask.val = 0x1c;
        break;
    case EM_ES_US_FOOT_ONLY:
        key_val.key_mask.val = 0x13;
        break;
    case EM_ES_US_HAND_AND_FOOT:
        key_val.key_mask.val = 0x1f;
        break;
    default:
        key_val.key_mask.val = 0x1f;
        break;
    }
}

void init_key_val(void)
{
    set_actived_mask(EM_US_HAND_AND_FOOT);
    key_val.key.val = 0;
    key_val.es_active_flag = false;
    key_val.us_max_active_flag = false;
    key_val.us_min_active_flag = false;
}

void check_key_actions(uint16_t key)
{
    key_val.key.val = key;
    key_val.ues_checked_flag = key_val.key.val & key_val.key_mask.val;
    if(key_val.ues_checked_flag & US_MAX_KEY_MASK_ALL)
    {
        // 需要区分切割和凝血操作
        key_val.us_max_active_flag = true;
    }
    else
    {
        key_val.us_max_active_flag = false;
    }

    if(key_val.ues_checked_flag & US_MIN_KEY_MASK_ALL)
    {
        // 需要区分切割和凝血操作
        key_val.us_min_active_flag = true;
    }
    else
    {
        key_val.us_min_active_flag = false;
    }

    if(key_val.ues_checked_flag & ES_KEY_MASK_ALL)
    {
        key_val.es_active_flag = true;
    }
    else
    {
        key_val.es_active_flag = false;
    }
}

bool is_us_max_actived(void)
{
    return key_val.us_max_active_flag;
}

bool is_us_min_actived(void)
{
    return key_val.us_min_active_flag;
}

bool is_es_min_actived(void)
{
    return key_val.es_active_flag;
}

// End of File:key.c

