// File:key.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "key.h"
ST_KEY_DATA key_val;

void set_actived_mask(ENUM_ACTIVED_CONTROL mask)
{
    switch(mask)
    {
        case EM_ALL_DISABLE:
        break;
        // US only
        case EM_US_HAND_ONLY:
        break;
        case EM_US_FOOT_ONLY:
        break;
        case EM_US_HAND_AND_FOOT:
        break;
        // ES only
        case EM_ES_ONLY:
        break;
        // US+ES
        case EM_ES_US_HAND_ONLY:
        break;
        case EM_ES_US_FOOT_ONLY:
        break;
        case EM_ES_US_HAND_AND_FOOT:
        break;
        default:
        break;
    }
}
// End of File:key.c

