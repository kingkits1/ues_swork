#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "fpga_control.h"

// us 允许fpga输出频率 = 1
// us 禁止fpga输出频率 = 0
void us_set_fpga_freq(ENUM_ACTIVITY act)
{
    PIout(7)=act;
}


