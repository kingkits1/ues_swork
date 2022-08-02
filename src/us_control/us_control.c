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
    us_9851_data.base.set_phase.phase = phase & 0x1f;
}

void Start_US_work(void)
{
    reset_us_control();
    // ad9851芯片复位
    us_reset_ad9851_chip();
    // 设置初始频率和相位
    init_ad9851_data(&us_9851_data, 55500 * 32); // 乘以32为FPGA特别需要
    // 设置初始功率输出值
    US_Set_Pwr(3000); // 贺工使用的是 3150
    // 打开功率输出通道
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1); //开启DAC通道1  //PA4-->US
    // 传输频率及相位数据
    ad9851_wr_data(&us_9851_data);
    // 使能数据
    us_update_ad9851_freq_data();
    // 似乎是使能FPGA？不需要在这里做
}


// 继电器为最后输出控制点，当系统控制失效时，
// 必须关闭继电器（act=0），以保护患者的安全。
// GPIO 控制是反着的，即 =1时是关闭
void us_set_electric_relay(ENUM_ACTIVITY act)
{
    if(act == EM_ACT_DISABLE)
    {
        PJout(12) = 1;
    }
    else
    {
        PJout(12) = 0;
    }
}

// 电源控制是反着的，即 =1时是关闭电源
void us_set_power_supply(ENUM_ACTIVITY act)
{
    if(act == EM_ACT_DISABLE)
    {
        PJout(14) = 1;
    }
    else
    {
        PJout(14) = 0;
    }
}

void us_set_output_enable(ENUM_ACTIVITY act)
{
    if(act) PIout(7) = 1;
    else PIout(7) = 0;
}

void reset_us_control(void)
{
    us_set_power_supply(EM_ACT_DISABLE);
    us_set_electric_relay(EM_ACT_DISABLE);
    us_set_output_enable(EM_ACT_DISABLE);
}


