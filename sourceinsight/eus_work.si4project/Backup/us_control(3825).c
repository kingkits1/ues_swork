#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "../hardware/AD9851_base.h"
#include "us_control.h"
extern DAC_HandleTypeDef hdac;
/******************************************************************************
*函数名称:DAC电压输出函数
*函数功能:在DAC1、DAC2的输出端口输出成比例的固定电压
*输入参数:u8 channel：通道号 1/2;u16 vol 目标电压值  0~3300,代表0~3.3V
*返回参数:无
*作    者:
*说    明:
******************************************************************************/
void US_Set_Pwr(uint16_t vol)
{
    float temp = (float)vol;    
    temp *= 1.2412121f; // = 4096/3300
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp); //12位右对齐数据格式设置DAC值
}

void US_Set_Freq(uint32_t freq)
{
    us_9851_data.base.set_frequency = freq;
}

void US_Set_Phase(uint8_t phase)
{
    us_9851_data.base.set_phase.phase = phase & 0c1f;
}

void Start_US_vork(void)
{
    // ad9851芯片复位
    us_reset_ad9851_chip();
    // 设置初始频率和相位
    init_ad9851_data(&us_9851_data,55500); // 乘以32为FPGA特别需要
    // 设置初始功率输出值
    US_Set_Pwr(3000); // 贺工使用的是 3150
    // 打开功率输出通道
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1); //开启DAC通道2  //PA5-->US
    // 传输频率及相位数据
    ad9851_wr_data(&us_9851_data);
    // 是能数据
    us_update_ad9851_freq_data();
    // 似乎是使能FPGA？不需要在这里做    
}


