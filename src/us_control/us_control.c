#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "../hardware/AD9851_base.h"
#include "us_control.h"
extern DAC_HandleTypeDef hdac;
/******************************************************************************
*��������:DAC��ѹ�������
*��������:��DAC1��DAC2������˿�����ɱ����Ĺ̶���ѹ
*�������:u8 channel��ͨ���� 1/2;u16 vol Ŀ���ѹֵ  0~3300,����0~3.3V
*���ز���:��
*��    ��:
*˵    ��:
******************************************************************************/
void US_Set_Pwr(uint16_t vol)
{
    float temp = (float)vol;
    temp *= 1.2412121f; // = 4096/3300
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp); //12λ�Ҷ������ݸ�ʽ����DACֵ
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
    // ad9851оƬ��λ
    us_reset_ad9851_chip();
    // ���ó�ʼƵ�ʺ���λ
    init_ad9851_data(&us_9851_data, 55500 * 32); // ����32ΪFPGA�ر���Ҫ
    // ���ó�ʼ�������ֵ
    US_Set_Pwr(3000); // �ع�ʹ�õ��� 3150
    // �򿪹������ͨ��
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1); //����DACͨ��1  //PA4-->US
    // ����Ƶ�ʼ���λ����
    ad9851_wr_data(&us_9851_data);
    // ʹ������
    us_update_ad9851_freq_data();
    // �ƺ���ʹ��FPGA������Ҫ��������
}


// �̵���Ϊ���������Ƶ㣬��ϵͳ����ʧЧʱ��
// ����رռ̵�����act=0�����Ա������ߵİ�ȫ��
// GPIO �����Ƿ��ŵģ��� =1ʱ�ǹر�
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

// ��Դ�����Ƿ��ŵģ��� =1ʱ�ǹرյ�Դ
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


