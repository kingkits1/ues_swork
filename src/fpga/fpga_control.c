#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "fpga_control.h"
#include "stm32f4xx_hal_sdram.h"

// us 允许fpga输出频率 = 1
// us 禁止fpga输出频率 = 0
void us_set_fpga_freq(ENUM_ACTIVITY act)
{
    PIout(7) = act;
}


// TODO: 未完成
// es 允许fpga输出频率 = 1
// es 禁止fpga输出频率 = 0
void es_set_fpga_freq(ENUM_ACTIVITY act)
{
    PIout(7) = act; // ！！！待定义
}
//
void set_fpga_ram_port(ENUM_ACTIVITY act)
{
    if(act == EM_ACT_DISABLE)
    {
        PIout(8) = 1;
    }
    else
    {
        PIout(8) = 0;
    }
}

