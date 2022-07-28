#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
CCMRAM static uint8_t debug_flag;
CCMRAM static uint16_t debug_data;

// 变量预定义 ---->
#if TEST__USART
#endif
#if TEST__SDRAM_FLAG
void fpga_read_data(void);
void clear_fpga_data(void);
#endif
#if TEST__UC_MIN_FLAG
#endif
#if TEST__UC_MAX_FLAG
#endif
#if TEST__EC_MIN_FLAG
#endif
#if TEST__EC_MAX_FLAG
#endif
#if TEST__SOUND_FLAG
void play_start_wav(ENUM_WAVE_TYPES wav_type);
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
    #if TEST__USART
    if(debug_flag==1)
    {
        debug_flag = 0;
        printf("TEST\n");
        dbg_printf("TEST\n");
    }    
    #endif
    #if TEST__SDRAM_FLAG
    if(debug_flag==1)
    {
        debug_flag = 0;
        clear_fpga_data();
        fpga_read_data();
    }
    #endif
    
    #if TEST__UC_MIN_FLAG
    #endif
    #if TEST__UC_MAX_FLAG
    #endif
    
    #if TEST__EC_MIN_FLAG
    #endif
    
    #if TEST__EC_MAX_FLAG
    #endif
    
    #if TEST__SOUND_FLAG
    if(debug_flag==1)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_US_CUTTING);
    }
    if(debug_flag==2)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_US_HEMOSTASIS);
    }
    if(debug_flag==3)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_US_CLOSING);
    }
    #endif    
}


