// File:fpga_manager.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "string.h"
#include "../sdram/sdram.h"
#include "fpga_manager.h"
static UNION_FPGA_MSG fpga_data_tmp;
UNION_FPGA_MSG fpga_data;
// 低电平使能
void set_fpga_read_port(bool act)
{
    if(act) PIout(8) = 0;
    else PIout(8) = 1;
}

void clear_fpga_data(void)
{
    fpga_data.buf[0] = 0;
    fpga_data.buf[1] = 0;
    fpga_data.buf[2] = 0;
    fpga_data.buf[3] = 0;
    fpga_data.buf[4] = 0;
    fpga_data.buf[5] = 0;
    fpga_data.buf[6] = 0;
    fpga_data.buf[7] = 0;
    fpga_data.buf[8] = 0;
    fpga_data.buf[9] = 0;
}



//
uint16_t fpga_data_check_sum1(void)
{
    uint16_t sum = 0;
    uint16_t i;
    for(i = 0; i < 8; i++)
    {
        sum ^= fpga_data_tmp.buf[i];
    }
    return sum;
}

uint16_t fpga_data_check_sum2(void)
{
    uint16_t sum = 0;
    sum ^= fpga_data_tmp.buf[4];
    sum ^= fpga_data_tmp.buf[5];
    sum ^= fpga_data_tmp.buf[8];
    return sum;
}

uint8_t fpga_read_data(void)
{
    FMC_SDRAM_ReadBuffer(fpga_data_tmp.buf, 0, 9);
    // check sum
    if(fpga_data_check_sum1() == 0)
    {
        memcpy(fpga_data.buf, fpga_data_tmp.buf, 16);
        return 0;
    }
    return 1;
}
// End of File: fpga_manager.c

