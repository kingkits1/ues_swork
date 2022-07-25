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
    es_9851_data.base.set_phase.phase = phase & 0x1f;
}

void Start_ES_work(void)
{
    // ad9851оƬ��λ
    es_reset_ad9851_chip();
    // ���ó�ʼƵ�ʺ���λ
    init_ad9851_data(&es_9851_data, 555000);
    // ���ó�ʼ�������ֵ
    ES_Set_Pwr(3100); // �ع�ʹ�õ��� 3150
    // �򿪹������ͨ��
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2); //����DACͨ��2  //PA5-->ES
    // ����Ƶ�ʼ���λ����
    ad9851_wr_data(&es_9851_data);
    // ֪ͨоƬʹ������
    es_update_ad9851_freq_data();
    // �ƺ���ʹ��FPGA������Ҫ��������
}

// TODO: δ���
// �̵���Ϊ���������Ƶ㣬��ϵͳ����ʧЧʱ��
// ����رռ̵�����act=0�����Ա������ߵİ�ȫ��
// GPIO �����Ƿ��ŵģ��� =1ʱ�ǹر�
void es_set_electric_relay(ENUM_ACTIVITY act)
{
    if(act == EM_DISABLE)
    {
        PDout(12) = 1;
    }
    else
    {
        PDout(12) = 0;
    }
}

// TODO: δ���
// ��Դ�����Ƿ��ŵģ��� =1ʱ�ǹرյ�Դ
void es_set_power_supply(ENUM_ACTIVITY act)
{
    if(act == EM_DISABLE)
    {
        PBout(5) = 1;
    }
    else
    {
        PBout(5) = 0;
    }
}

