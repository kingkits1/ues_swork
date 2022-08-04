#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "sai.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

extern SAI_HandleTypeDef hsai_BlockA1;        //SAI1 Block A句柄
extern DMA_HandleTypeDef hdma_sai1_a;   //DMA发送句柄

//SAI Block A初始化,I2S,飞利浦标准
//mode:工作模式,可以设置:SAI_MODEMASTER_TX/SAI_MODEMASTER_RX/SAI_MODESLAVE_TX/SAI_MODESLAVE_RX
//cpol:数据在时钟的上升/下降沿选通，可以设置：SAI_CLOCKSTROBING_FALLINGEDGE/SAI_CLOCKSTROBING_RISINGEDGE
//datalen:数据大小,可以设置：SAI_DATASIZE_8/10/16/20/24/32
void SAIA_Init(uint32_t mode, uint32_t cpol, uint32_t datalen)
{
    HAL_SAI_DeInit(&hsai_BlockA1);                          //清除以前的配置
    hsai_BlockA1.Instance = SAI1_Block_A;                   //SAI1 Bock A
    hsai_BlockA1.Init.AudioMode = mode;                     //设置SAI1工作模式
    hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;           //音频模块异步
    hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE; //立即驱动音频模块输出
    hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE; //使能主时钟分频器(MCKDIV)
    hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF; //设置FIFO阈值,1/4 FIFO
    hsai_BlockA1.Init.ClockSource = SAI_CLKSOURCE_PLLI2S;   //SIA时钟源为PLL2S
    hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;      //立体声模式
    hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;         //设置SAI1协议为:自由协议(支持I2S/LSB/MSB/TDM/PCM/DSP等协议)
    hsai_BlockA1.Init.DataSize = datalen;                   //设置数据大小
    hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;          //数据MSB位优先
    hsai_BlockA1.Init.ClockStrobing = cpol;                 //数据在时钟的上升/下降沿选通

    //帧设置
    hsai_BlockA1.FrameInit.FrameLength = 64;                //设置帧长度为64,左通道32个SCK,右通道32个SCK.
    hsai_BlockA1.FrameInit.ActiveFrameLength = 32;          //设置帧同步有效电平长度,在I2S模式下=1/2帧长.
    hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION; //FS信号为SOF信号+通道识别信号
    hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;  //FS低电平有效(下降沿)
    hsai_BlockA1.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT; //在slot0的第一位的前一位使能FS,以匹配飞利浦标准

    //SLOT设置
    hsai_BlockA1.SlotInit.FirstBitOffset = 0;               //slot偏移(FBOFF)为0
    hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_32B;      //slot大小为32位
    hsai_BlockA1.SlotInit.SlotNumber = 2;                   //slot数为2个
    hsai_BlockA1.SlotInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1; //使能slot0和slot1

    HAL_SAI_Init(&hsai_BlockA1);                            //初始化SAI
    __HAL_SAI_ENABLE(&hsai_BlockA1);                        //使能SAI
}

#if 0
//SAI底层驱动，引脚配置，时钟使能
//此函数会被HAL_SAI_Init()调用
//hsdram:SAI句柄
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_SAI1_CLK_ENABLE();                //使能SAI1时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();               //使能GPIOE时钟

    //初始化PE2,3,4,5,6
    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;        //推挽复用
    GPIO_Initure.Pull = GPIO_PULLUP;            //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;       //高速
    GPIO_Initure.Alternate = GPIO_AF6_SAI1;     //复用为SAI
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);        //初始化
}
#endif
//SAI Block A采样率设置
//采样率计算公式:
//MCKDIV!=0: Fs=SAI_CK_x/[512*MCKDIV]
//MCKDIV==0: Fs=SAI_CK_x/256
//SAI_CK_x=(HSE/pllm)*PLLI2SN/PLLI2SQ/(PLLI2SDIVQ+1)
//一般HSE=25Mhz
//pllm:在Stm32_Clock_Init设置的时候确定，一般是25
//PLLI2SN:一般是192~432
//PLLI2SQ:2~15
//PLLI2SDIVQ:0~31
//MCKDIV:0~15
//SAI A分频系数表@pllm=25,HSE=25Mhz,即vco输入频率为1Mhz
#if 0
const uint16_t SAI_PSC_TBL[][5] =
{
    {800 , 344, 7, 0, 12},	//8Khz采样率
    {1102, 429, 2, 18, 2},	//11.025Khz采样率
    {1600, 344, 7, 0, 6},	//16Khz采样率
    {2205, 429, 2, 18, 1},	//22.05Khz采样率
    {3200, 344, 7, 0, 3},	//32Khz采样率
    {4410, 429, 2, 18, 0},	//44.1Khz采样率
    {4800, 344, 7, 0, 2},	//48Khz采样率
    {8820, 271, 2, 2, 1},	//88.2Khz采样率
    {9600, 344, 7, 0, 1},	//96Khz采样率
    {17640, 271, 2, 2, 0},	//176.4Khz采样率
    {19200, 344, 7, 0, 0},	//192Khz采样率
};
#else
const uint16_t SAI_PSC_TBL[][5] =
{
    {800 , 344, 7, 0, 12},	//8Khz采样率
    {1102, 429, 2, 18, 2},	//11.025Khz采样率
    {1600, 344, 7, 0, 6},	//16Khz采样率
    {2205, 429, 2, 18, 1},	//22.05Khz采样率
    {3200, 344, 7, 0, 3},	//32Khz采样率
    {4410, 429, 2, 18, 0},	//44.1Khz采样率
    {4800, 344, 7, 0, 2},	//48Khz采样率
    {8820, 271, 2, 2, 1},	//88.2Khz采样率
    {9600, 344, 7, 0, 1},	//96Khz采样率
    {17640, 271, 2, 2, 0},	//176.4Khz采样率
    {19200, 344, 7, 0, 0},	//192Khz采样率
};

#endif
//开启SAI的DMA功能,HAL库没有提供此函数
//因此我们需要自己操作寄存器编写一个
void SAIA_DMA_Enable(void)
{
    uint32_t tempreg = 0;
    tempreg = SAI1_Block_A->CR1;        //先读出以前的设置
    tempreg |= 1 << 17;					 //使能DMA
    SAI1_Block_A->CR1 = tempreg;		  //写入CR1寄存器中
}

//设置SAIA的采样率(@MCKEN)
//samplerate:采样率,单位:Hz
//返回值:0,设置成功;1,无法设置.
uint8_t SAIA_SampleRate_Set(uint32_t samplerate)
{
    uint8_t i = 0;

    RCC_PeriphCLKInitTypeDef RCCSAI1_Sture;
    for(i = 0; i < (sizeof(SAI_PSC_TBL) / 10); i++) //看看改采样率是否可以支持
    {
        if((samplerate / 10) == SAI_PSC_TBL[i][0])break;
    }
    if(i == (sizeof(SAI_PSC_TBL) / 10))return 1; //搜遍了也找不到
    RCCSAI1_Sture.PeriphClockSelection = RCC_PERIPHCLK_SAI_PLLI2S; //外设时钟源选择
    RCCSAI1_Sture.PLLI2S.PLLI2SN = (uint32_t)SAI_PSC_TBL[i][1];      //设置PLLI2SN
    RCCSAI1_Sture.PLLI2S.PLLI2SQ = (uint32_t)SAI_PSC_TBL[i][2];      //设置PLLI2SQ
    //设置PLLI2SDivQ的时候SAI_PSC_TBL[i][3]要加1，因为HAL库中会在把PLLI2SDivQ赋给寄存器DCKCFGR的时候减1
    RCCSAI1_Sture.PLLI2SDivQ = SAI_PSC_TBL[i][3] + 1;           //设置PLLI2SDIVQ
    HAL_RCCEx_PeriphCLKConfig(&RCCSAI1_Sture);                  //设置时钟
	delay_us(10);
	
#if 1
    __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(RCC_SAIACLKSOURCE_PLLI2S); //设置SAI1时钟来源为PLLI2SQ
    __HAL_SAI_DISABLE(&hsai_BlockA1);                          //关闭SAI
    hsai_BlockA1.Init.AudioFrequency = samplerate;             //设置播放频率
    HAL_SAI_Init(&hsai_BlockA1);                               //初始化SAI
#endif
    SAIA_DMA_Enable();                                          //开启SAI的DMA功能
    __HAL_SAI_ENABLE(&hsai_BlockA1);                           //开启SAI
    return 0;
}

//SAIA TX DMA配置
//设置为双缓冲模式,并开启DMA传输完成中断
//buf0:M0AR地址.
//buf1:M1AR地址.
//num:每次传输数据量
//width:位宽(存储器和外设,同时设置),0,8位;1,16位;2,32位;
void SAIA_TX_DMA_Init(uint8_t *buf0, uint8_t *buf1, uint16_t num, uint8_t width)
{
    uint32_t memwidth = 0, perwidth = 0; //外设和存储器位宽
    switch(width)
    {
    case 0:         //8位
        memwidth = DMA_MDATAALIGN_BYTE;
        perwidth = DMA_PDATAALIGN_BYTE;
        break;
    case 1:         //16位
        memwidth = DMA_MDATAALIGN_HALFWORD;
        perwidth = DMA_PDATAALIGN_HALFWORD;
        break;
    case 2:         //32位
        memwidth = DMA_MDATAALIGN_WORD;
        perwidth = DMA_PDATAALIGN_WORD;
        break;

    }
    __HAL_RCC_DMA2_CLK_ENABLE();                                    //使能DMA2时钟
    __HAL_LINKDMA(&hsai_BlockA1, hdmatx, hdma_sai1_a);       //将DMA与SAI联系起来
    hdma_sai1_a.Instance = DMA2_Stream3;                     //DMA2数据流3
    hdma_sai1_a.Init.Channel = DMA_CHANNEL_0;                //通道0
    hdma_sai1_a.Init.Direction = DMA_MEMORY_TO_PERIPH;       //存储器到外设模式
    hdma_sai1_a.Init.PeriphInc = DMA_PINC_DISABLE;           //外设非增量模式
    hdma_sai1_a.Init.MemInc = DMA_MINC_ENABLE;               //存储器增量模式
    hdma_sai1_a.Init.PeriphDataAlignment = perwidth;         //外设数据长度:16/32位
    hdma_sai1_a.Init.MemDataAlignment = memwidth;            //存储器数据长度:16/32位
    hdma_sai1_a.Init.Mode = DMA_CIRCULAR;                    //使用循环模式
    hdma_sai1_a.Init.Priority = DMA_PRIORITY_HIGH;           //高优先级
    hdma_sai1_a.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        //不使用FIFO
    hdma_sai1_a.Init.MemBurst = DMA_MBURST_SINGLE;           //存储器单次突发传输
    hdma_sai1_a.Init.PeriphBurst = DMA_PBURST_SINGLE;        //外设突发单次传输
    HAL_DMA_DeInit(&hdma_sai1_a);                            //先清除以前的设置
    HAL_DMA_Init(&hdma_sai1_a);	                            //初始化DMA

    HAL_DMAEx_MultiBufferStart(&hdma_sai1_a, (uint32_t)buf0, (uint32_t)&SAI1_Block_A->DR, (uint32_t)buf1, num); //开启双缓冲
    __HAL_DMA_DISABLE(&hdma_sai1_a);                         //先关闭DMA
    delay_us(10);                                                   //10us延时，防止-O2优化出问题
    __HAL_DMA_ENABLE_IT(&hdma_sai1_a, DMA_IT_TC);            //开启传输完成中断
    __HAL_DMA_CLEAR_FLAG(&hdma_sai1_a, DMA_FLAG_TCIF3_7);    //清除DMA传输完成中断标志位
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);                  //DMA中断优先级
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

//SAI DMA回调函数指针
void (*sai_tx_callback)(void);	//TX回调函数
//DMA2_Stream3中断服务函数
void DMA2_Stream3_IRQHandler(void)
{
    if(__HAL_DMA_GET_FLAG(&hdma_sai1_a, DMA_FLAG_TCIF3_7) != RESET) //DMA传输完成
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_sai1_a, DMA_FLAG_TCIF3_7);    //清除DMA传输完成中断标志位
        sai_tx_callback();	//执行回调函数,读取数据等操作在这里面处理
    }
}
//SAI开始播放
void SAI_Play_Start(void)
{
    __HAL_DMA_ENABLE(&hdma_sai1_a);//开启DMA TX传输
}
//关闭I2S播放
void SAI_Play_Stop(void)
{
    __HAL_DMA_DISABLE(&hdma_sai1_a);  //结束播放
}

