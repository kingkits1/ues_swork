#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "fpga_control.h"

// us ����fpga���Ƶ�� = 1
// us ��ֹfpga���Ƶ�� = 0
void us_set_fpga_freq(ENUM_ACTIVITY act)
{
    PIout(7)=act;
}


