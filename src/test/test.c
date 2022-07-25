#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
CCMRAM static uint8_t debug_flag;
CCMRAM static uint16_t debug_data;

// 变量预定义 ---->
#if EM_TEST_SDRAM_FLAG
#endif
#if EM_TEST_UC_MIN_FLAG
#endif
#if EM_TEST_UC_MAX_FLAG
#endif
#if EM_TEST_EC_MIN_FLAG
#endif
#if EM_TEST_EC_MAX_FLAG
#endif
#if EM_TEST_SOUND_FLAG
#endif


void set_debug_flag(uint8_t flag)
{
    debug_flag = flag;
}

void set_debug_data(uint16_t data)
{
    debug_data = data;
}


void test_manager(void)
{
#if EM_TEST_SDRAM_FLAG
#endif
#if EM_TEST_UC_MIN_FLAG
#endif
#if EM_TEST_UC_MAX_FLAG
#endif
#if EM_TEST_EC_MIN_FLAG
#endif
#if EM_TEST_EC_MAX_FLAG
#endif
#if EM_TEST_SOUND_FLAG
#endif    
}


