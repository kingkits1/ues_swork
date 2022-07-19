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
    us_9851_data.base.set_phase.phase = phase & 0c1f;
}

void Start_US_vork(void)
{
    // ad9851оƬ��λ
    us_reset_ad9851_chip();
    // ���ó�ʼƵ�ʺ���λ
    init_ad9851_data(&us_9851_data,55500); // ����32ΪFPGA�ر���Ҫ
    // ���ó�ʼ�������ֵ
    US_Set_Pwr(3000); // �ع�ʹ�õ��� 3150
    // �򿪹������ͨ��
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1); //����DACͨ��2  //PA5-->US
    // ����Ƶ�ʼ���λ����
    ad9851_wr_data(&us_9851_data);
    // ��������
    us_update_ad9851_freq_data();
    // �ƺ���ʹ��FPGA������Ҫ��������    
}


