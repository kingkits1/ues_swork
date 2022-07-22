#include "myiic3.h"
//#include "delay.h"

//IIC��ʼ��
void IIC3_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();   //ʹ��GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    //PA8��ʼ������
    GPIO_Initure.Pin = GPIO_PIN_8;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_Initure.Pull = GPIO_PULLUP;        //����
    GPIO_Initure.Speed = GPIO_SPEED_FAST;   //����
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    //PC9��ʼ������
    GPIO_Initure.Pin = GPIO_PIN_9;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_Initure.Pull = GPIO_PULLUP;        //����
    GPIO_Initure.Speed = GPIO_SPEED_FAST;   //����
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    IIC3_SDA = 1;
    IIC3_SCL = 1;
}

//����IIC��ʼ�ź�
void IIC3_Start(void)
{
    SDA3_OUT();     //sda�����
    IIC3_SDA = 1;
    IIC3_SCL = 1;
    delay_us(4);
    IIC3_SDA = 0; //START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC3_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC3_Stop(void)
{
    SDA3_OUT();//sda�����
    IIC3_SCL = 0;
    IIC3_SDA = 0; //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC3_SCL = 1;
    delay_us(4);
    IIC3_SDA = 1; //����I2C���߽����ź�
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC3_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA3_IN();      //SDA����Ϊ����
    IIC3_SDA = 1;
    delay_us(1);
    IIC3_SCL = 1;
    delay_us(1);
    while(READ_SDA3)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC3_Stop();
            return 1;
        }
    }
    IIC3_SCL = 0; //ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC3_Ack(void)
{
    IIC3_SCL = 0;
    SDA3_OUT();
    IIC3_SDA = 0;
    delay_us(2);
    IIC3_SCL = 1;
    delay_us(2);
    IIC3_SCL = 0;
}
//������ACKӦ��
void IIC3_NAck(void)
{
    IIC3_SCL = 0;
    SDA3_OUT();
    IIC3_SDA = 1;
    delay_us(2);
    IIC3_SCL = 1;
    delay_us(2);
    IIC3_SCL = 0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC3_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA3_OUT();
    IIC3_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
    for(t = 0; t < 8; t++)
    {
        IIC3_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        delay_us(2);   //��TEA5767��������ʱ���Ǳ����
        IIC3_SCL = 1;
        delay_us(2);
        IIC3_SCL = 0;
        delay_us(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC3_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA3_IN();//SDA����Ϊ����
    for(i = 0; i < 8; i++ )
    {
        IIC3_SCL = 0;
        delay_us(2);
        IIC3_SCL = 1;
        receive <<= 1;
        if(READ_SDA3)receive++;
        delay_us(1);
    }
    if (!ack)
        IIC3_NAck();//����nACK
    else
        IIC3_Ack(); //����ACK
    return receive;
}


