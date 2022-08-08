#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "sai.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

extern SAI_HandleTypeDef hsai_BlockA1;        //SAI1 Block A���
extern DMA_HandleTypeDef hdma_sai1_a;   //DMA���;��

//SAI Block A��ʼ��,I2S,�����ֱ�׼
//mode:����ģʽ,��������:SAI_MODEMASTER_TX/SAI_MODEMASTER_RX/SAI_MODESLAVE_TX/SAI_MODESLAVE_RX
//cpol:������ʱ�ӵ�����/�½���ѡͨ���������ã�SAI_CLOCKSTROBING_FALLINGEDGE/SAI_CLOCKSTROBING_RISINGEDGE
//datalen:���ݴ�С,�������ã�SAI_DATASIZE_8/10/16/20/24/32
void SAIA_Init(uint32_t mode, uint32_t cpol, uint32_t datalen)
{
    HAL_SAI_DeInit(&hsai_BlockA1);                          //�����ǰ������
    hsai_BlockA1.Instance = SAI1_Block_A;                   //SAI1 Bock A
    hsai_BlockA1.Init.AudioMode = mode;                     //����SAI1����ģʽ
    hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;           //��Ƶģ���첽
    hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE; //����������Ƶģ�����
    hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE; //ʹ����ʱ�ӷ�Ƶ��(MCKDIV)
    hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF; //����FIFO��ֵ,1/4 FIFO
    hsai_BlockA1.Init.ClockSource = SAI_CLKSOURCE_PLLI2S;   //SIAʱ��ԴΪPLL2S
    hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;      //������ģʽ
    hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;         //����SAI1Э��Ϊ:����Э��(֧��I2S/LSB/MSB/TDM/PCM/DSP��Э��)
    hsai_BlockA1.Init.DataSize = datalen;                   //�������ݴ�С
    hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;          //����MSBλ����
    hsai_BlockA1.Init.ClockStrobing = cpol;                 //������ʱ�ӵ�����/�½���ѡͨ

    //֡����
    hsai_BlockA1.FrameInit.FrameLength = 64;                //����֡����Ϊ64,��ͨ��32��SCK,��ͨ��32��SCK.
    hsai_BlockA1.FrameInit.ActiveFrameLength = 32;          //����֡ͬ����Ч��ƽ����,��I2Sģʽ��=1/2֡��.
    hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION; //FS�ź�ΪSOF�ź�+ͨ��ʶ���ź�
    hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;  //FS�͵�ƽ��Ч(�½���)
    hsai_BlockA1.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT; //��slot0�ĵ�һλ��ǰһλʹ��FS,��ƥ������ֱ�׼

    //SLOT����
    hsai_BlockA1.SlotInit.FirstBitOffset = 0;               //slotƫ��(FBOFF)Ϊ0
    hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_32B;      //slot��СΪ32λ
    hsai_BlockA1.SlotInit.SlotNumber = 2;                   //slot��Ϊ2��
    hsai_BlockA1.SlotInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1; //ʹ��slot0��slot1

    HAL_SAI_Init(&hsai_BlockA1);                            //��ʼ��SAI
    __HAL_SAI_ENABLE(&hsai_BlockA1);                        //ʹ��SAI
}

#if 0
//SAI�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_SAI_Init()����
//hsdram:SAI���
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_SAI1_CLK_ENABLE();                //ʹ��SAI1ʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();               //ʹ��GPIOEʱ��

    //��ʼ��PE2,3,4,5,6
    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;        //���츴��
    GPIO_Initure.Pull = GPIO_PULLUP;            //����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;       //����
    GPIO_Initure.Alternate = GPIO_AF6_SAI1;     //����ΪSAI
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);        //��ʼ��
}
#endif
//SAI Block A����������
//�����ʼ��㹫ʽ:
//MCKDIV!=0: Fs=SAI_CK_x/[512*MCKDIV]
//MCKDIV==0: Fs=SAI_CK_x/256
//SAI_CK_x=(HSE/pllm)*PLLI2SN/PLLI2SQ/(PLLI2SDIVQ+1)
//һ��HSE=25Mhz
//pllm:��Stm32_Clock_Init���õ�ʱ��ȷ����һ����25
//PLLI2SN:һ����192~432
//PLLI2SQ:2~15
//PLLI2SDIVQ:0~31
//MCKDIV:0~15
//SAI A��Ƶϵ����@pllm=25,HSE=25Mhz,��vco����Ƶ��Ϊ1Mhz
#if 0
const uint16_t SAI_PSC_TBL[][5] =
{
    {800 , 344, 7, 0, 12},	//8Khz������
    {1102, 429, 2, 18, 2},	//11.025Khz������
    {1600, 344, 7, 0, 6},	//16Khz������
    {2205, 429, 2, 18, 1},	//22.05Khz������
    {3200, 344, 7, 0, 3},	//32Khz������
    {4410, 429, 2, 18, 0},	//44.1Khz������
    {4800, 344, 7, 0, 2},	//48Khz������
    {8820, 271, 2, 2, 1},	//88.2Khz������
    {9600, 344, 7, 0, 1},	//96Khz������
    {17640, 271, 2, 2, 0},	//176.4Khz������
    {19200, 344, 7, 0, 0},	//192Khz������
};
#else
const uint16_t SAI_PSC_TBL[][5] =
{
    {800 , 344, 7, 0, 12},	//8Khz������
    {1102, 429, 2, 18, 2},	//11.025Khz������
    {1600, 344, 7, 0, 6},	//16Khz������
    {2205, 429, 2, 18, 1},	//22.05Khz������
    {3200, 344, 7, 0, 3},	//32Khz������
    {4410, 429, 2, 18, 0},	//44.1Khz������
    {4800, 344, 7, 0, 2},	//48Khz������
    {8820, 271, 2, 2, 1},	//88.2Khz������
    {9600, 344, 7, 0, 1},	//96Khz������
    {17640, 271, 2, 2, 0},	//176.4Khz������
    {19200, 344, 7, 0, 0},	//192Khz������
};

#endif
//����SAI��DMA����,HAL��û���ṩ�˺���
//���������Ҫ�Լ������Ĵ�����дһ��
void SAIA_DMA_Enable(void)
{
    uint32_t tempreg = 0;
    tempreg = SAI1_Block_A->CR1;        //�ȶ�����ǰ������
    tempreg |= 1 << 17;					 //ʹ��DMA
    SAI1_Block_A->CR1 = tempreg;		  //д��CR1�Ĵ�����
}

//����SAIA�Ĳ�����(@MCKEN)
//samplerate:������,��λ:Hz
//����ֵ:0,���óɹ�;1,�޷�����.
uint8_t SAIA_SampleRate_Set(uint32_t samplerate)
{
    uint8_t i = 0;

    RCC_PeriphCLKInitTypeDef RCCSAI1_Sture;
    for(i = 0; i < (sizeof(SAI_PSC_TBL) / 10); i++) //�����Ĳ������Ƿ����֧��
    {
        if((samplerate / 10) == SAI_PSC_TBL[i][0])break;
    }
    if(i == (sizeof(SAI_PSC_TBL) / 10))return 1; //�ѱ���Ҳ�Ҳ���
    RCCSAI1_Sture.PeriphClockSelection = RCC_PERIPHCLK_SAI_PLLI2S; //����ʱ��Դѡ��
    RCCSAI1_Sture.PLLI2S.PLLI2SN = (uint32_t)SAI_PSC_TBL[i][1];      //����PLLI2SN
    RCCSAI1_Sture.PLLI2S.PLLI2SQ = (uint32_t)SAI_PSC_TBL[i][2];      //����PLLI2SQ
    //����PLLI2SDivQ��ʱ��SAI_PSC_TBL[i][3]Ҫ��1����ΪHAL���л��ڰ�PLLI2SDivQ�����Ĵ���DCKCFGR��ʱ���1
    RCCSAI1_Sture.PLLI2SDivQ = SAI_PSC_TBL[i][3] + 1;           //����PLLI2SDIVQ
    HAL_RCCEx_PeriphCLKConfig(&RCCSAI1_Sture);                  //����ʱ��
    delay_us(10);

#if 1
    __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(RCC_SAIACLKSOURCE_PLLI2S); //����SAI1ʱ����ԴΪPLLI2SQ
    __HAL_SAI_DISABLE(&hsai_BlockA1);                          //�ر�SAI
    hsai_BlockA1.Init.AudioFrequency = samplerate;             //���ò���Ƶ��
    HAL_SAI_Init(&hsai_BlockA1);                               //��ʼ��SAI
#endif
    SAIA_DMA_Enable();                                          //����SAI��DMA����
    __HAL_SAI_ENABLE(&hsai_BlockA1);                           //����SAI
    return 0;
}

//SAIA TX DMA����
//����Ϊ˫����ģʽ,������DMA��������ж�
//buf0:M0AR��ַ.
//buf1:M1AR��ַ.
//num:ÿ�δ���������
//width:λ��(�洢��������,ͬʱ����),0,8λ;1,16λ;2,32λ;
void SAIA_TX_DMA_Init(uint8_t *buf0, uint8_t *buf1, uint16_t num, uint8_t width)
{
    uint32_t memwidth = 0, perwidth = 0; //����ʹ洢��λ��
    switch(width)
    {
    case 0:         //8λ
        memwidth = DMA_MDATAALIGN_BYTE;
        perwidth = DMA_PDATAALIGN_BYTE;
        break;
    case 1:         //16λ
        memwidth = DMA_MDATAALIGN_HALFWORD;
        perwidth = DMA_PDATAALIGN_HALFWORD;
        break;
    case 2:         //32λ
        memwidth = DMA_MDATAALIGN_WORD;
        perwidth = DMA_PDATAALIGN_WORD;
        break;

    }
    __HAL_RCC_DMA2_CLK_ENABLE();                                    //ʹ��DMA2ʱ��
    __HAL_LINKDMA(&hsai_BlockA1, hdmatx, hdma_sai1_a);       //��DMA��SAI��ϵ����
    hdma_sai1_a.Instance = DMA2_Stream3;                     //DMA2������3
    hdma_sai1_a.Init.Channel = DMA_CHANNEL_0;                //ͨ��0
    hdma_sai1_a.Init.Direction = DMA_MEMORY_TO_PERIPH;       //�洢��������ģʽ
    hdma_sai1_a.Init.PeriphInc = DMA_PINC_DISABLE;           //���������ģʽ
    hdma_sai1_a.Init.MemInc = DMA_MINC_ENABLE;               //�洢������ģʽ
    hdma_sai1_a.Init.PeriphDataAlignment = perwidth;         //�������ݳ���:16/32λ
    hdma_sai1_a.Init.MemDataAlignment = memwidth;            //�洢�����ݳ���:16/32λ
    hdma_sai1_a.Init.Mode = DMA_CIRCULAR;                    //ʹ��ѭ��ģʽ
    hdma_sai1_a.Init.Priority = DMA_PRIORITY_HIGH;           //�����ȼ�
    hdma_sai1_a.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        //��ʹ��FIFO
    hdma_sai1_a.Init.MemBurst = DMA_MBURST_SINGLE;           //�洢������ͻ������
    hdma_sai1_a.Init.PeriphBurst = DMA_PBURST_SINGLE;        //����ͻ�����δ���
    HAL_DMA_DeInit(&hdma_sai1_a);                            //�������ǰ������
    HAL_DMA_Init(&hdma_sai1_a);	                            //��ʼ��DMA

    HAL_DMAEx_MultiBufferStart(&hdma_sai1_a, (uint32_t)buf0, (uint32_t)&SAI1_Block_A->DR, (uint32_t)buf1, num); //����˫����
    __HAL_DMA_DISABLE(&hdma_sai1_a);                         //�ȹر�DMA
    delay_us(10);                                                   //10us��ʱ����ֹ-O2�Ż�������
    __HAL_DMA_ENABLE_IT(&hdma_sai1_a, DMA_IT_TC);            //������������ж�
    __HAL_DMA_CLEAR_FLAG(&hdma_sai1_a, DMA_FLAG_TCIF3_7);    //���DMA��������жϱ�־λ
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);                  //DMA�ж����ȼ�
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

//SAI DMA�ص�����ָ��
void (*sai_tx_callback)(void);	//TX�ص�����
//DMA2_Stream3�жϷ�����
void DMA2_Stream3_IRQHandler(void)
{
    if(__HAL_DMA_GET_FLAG(&hdma_sai1_a, DMA_FLAG_TCIF3_7) != RESET) //DMA�������
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_sai1_a, DMA_FLAG_TCIF3_7);    //���DMA��������жϱ�־λ
        sai_tx_callback();	//ִ�лص�����,��ȡ���ݵȲ����������洦��
    }
}
//SAI��ʼ����
void SAI_Play_Start(void)
{
    __HAL_DMA_ENABLE(&hdma_sai1_a);//����DMA TX����
}
//�ر�I2S����
void SAI_Play_Stop(void)
{
    __HAL_DMA_DISABLE(&hdma_sai1_a);  //��������
}

