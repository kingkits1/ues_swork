#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "../hardware/AD9851_base.h"
#include "es_control.h"
extern DAC_HandleTypeDef hdac;
/******************************************************************************
*��������:DAC��ѹ�������
*��������:��DAC1��DAC2������˿�����ɱ����Ĺ̶���ѹ
*�������:u8 channel��ͨ���� 1/2;u16 vol Ŀ���ѹֵ  0~3300,����0~3.3V
*���ز���:��
*��    ��:
*˵    ��:
******************************************************************************/
void ES_Set_Pwr(uint16_t vol)
{
    float temp = (float)vol;    
    temp *= 1.2412121f; // = 4096/3300
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, temp); //12λ�Ҷ������ݸ�ʽ����DACֵ
}

void ES_Set_Freq(uint32_t freq)
{
    es_9851_data.base.set_frequency = freq;
}

void ES_Set_Phase(uint8_t phase)
{
    es_9851_data.base.set_phase.phase = phase & 0c1f;
}

void Start_ES_vork(void)
{
    // ad9851оƬ��λ
    es_reset_ad9851_chip();
    // ���ó�ʼƵ�ʺ���λ
    init_ad9851_data(&es_9851_data,555000); 
    // ���ó�ʼ�������ֵ
    ES_Set_Pwr(3000); // �ع�ʹ�õ��� 3150
    // �򿪹������ͨ��
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2); //����DACͨ��2  //PA5-->ES
    // ����Ƶ�ʼ���λ����
    ad9851_wr_data(&es_9851_data);
    // ��������
    es_update_ad9851_freq_data();
    // �ƺ���ʹ��FPGA������Ҫ��������    
}

