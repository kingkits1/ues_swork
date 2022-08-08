// File:fpga_manager.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "string.h"
#include "fpga_manager.h"
#include "fpga_control.h"
#include "../key/key.h"
static UNION_FPGA_MSG fpga_data_tmp;
UNION_FPGA_MSG fpga_data;
// 低电平使能
void set_fpga_read_port(bool act)
{
    if(act) PIout(8) = 0;
    else PIout(8) = 1;
}

#define FPGA_BASE        ((unsigned int)(0x6007FE00))
uint16_t FPGA_Read(uint8_t addr)
{
    vu16 *RAM, data;
    RAM = (volatile uint16_t *)(FPGA_BASE + (addr << 1));
    data = *RAM;
    return data;
}

void fpga_read_buf(uint16_t *pBuffer, uint32_t ReadAddr, uint32_t len)
{
    uint32_t r_ptr = ReadAddr << 1;
    set_fpga_read_port(true);
    while(len--)
    {
        *pBuffer++ = *((volatile uint16_t *)(FPGA_BASE + r_ptr));
        r_ptr += 2;
    }
    set_fpga_read_port(false);
}
void fpga_write_buf(uint16_t *pBuffer, uint32_t WriteAddr, uint32_t len)
{
    uint32_t w_ptr = WriteAddr << 1;
    while(len--)
    {
        *((volatile uint16_t *)(FPGA_BASE + w_ptr)) = *pBuffer;
        w_ptr += 2;
        pBuffer++;
    }
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


void refresh_handler_status(void)
{
    switch(fpga_data.data.hand_msg)
    {
    case EM_HANDLER_STATUS_MIN: //        = 1, // MIN按下
        set_handmaxmin_val(0, 1);
        set_hand_close_status(false);
        break;
    case EM_HANDLER_STATUS_MAX: //        = 2, // MAX按下
        set_handmaxmin_val(1, 0);
        set_hand_close_status(false);
        break;
    case EM_HANDLER_STATUS_MIN_CLOSE://   = 3, // MIN按下且钳口闭合
        set_handmaxmin_val(0, 1);
        set_hand_close_status(true);
        break;
    case EM_HANDLER_STATUS_MAX_CLOSE://   = 4, // MAX按下且钳口闭合
        set_handmaxmin_val(1, 0);
        set_hand_close_status(true);
        break;
    case EM_HANDLER_STATUS_ONLY_Close://  = 5, // 仅钳口闭合，MAX/MIN未按下，可能脚踏使用
        set_handmaxmin_val(0, 0);
        set_hand_close_status(true);
        break;
    case EM_HANDLER_STATUS_CONNECT://     = 6, //手柄接通	6
        set_handmaxmin_val(0, 0);
        set_hand_close_status(false);
        break;
    case EM_HANDLER_STATUS_DISCONNECT://  = 9  //手柄断开	9
        set_handmaxmin_val(0, 0);
        set_hand_close_status(false);
        break;
    default:
        set_handmaxmin_val(0, 0);
        set_hand_close_status(false);
        break;
    }
}
uint8_t fpga_read_data(void)
{
    fpga_read_buf(fpga_data_tmp.buf, 0, 9);
    // check sum
    if(fpga_data_check_sum1() == 0)
    {
        memcpy(fpga_data.buf, fpga_data_tmp.buf, 16);
        refresh_handler_status();
        return 0;
    }
    return 1;
}
// End of File: fpga_manager.c

