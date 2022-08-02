#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "fpga_control.h"
#include "stm32f4xx_hal_sdram.h"

// us ����fpga���Ƶ�� = 1
// us ��ֹfpga���Ƶ�� = 0
void us_set_fpga_freq(ENUM_ACTIVITY act)
{
    PIout(7) = act;
}


// TODO: δ���
// es ����fpga���Ƶ�� = 1
// es ��ֹfpga���Ƶ�� = 0
void es_set_fpga_freq(ENUM_ACTIVITY act)
{
    PIout(7) = act; // ������������
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

