#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

//IO��������
//#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;}	//PH5����ģʽ
//#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} //PH5���ģʽ
//IO����
//#define IIC_SCL   PHout(4) //SCL
//#define IIC_SDA   PHout(5) //SDA
//#define READ_SDA  PHin(5)  //����SDA

#define SDA3_IN()  {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	//PH5����ģʽ
#define SDA3_OUT() {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} //PH5���ģʽ

#define IIC3_SCL   PAout(8) //SCL     //kangyingkui 2021.03.10
#define IIC3_SDA   PCout(9) //SDA
#define READ_SDA3  PCin(9)  //����SDA

//IIC���в�������
void IIC3_Init(void);                //��ʼ��IIC��IO��
void IIC3_Start(void);				//����IIC��ʼ�ź�
void IIC3_Stop(void);	  			//����IICֹͣ�ź�
void IIC3_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC3_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC3_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC3_Ack(void);					//IIC����ACK�ź�
void IIC3_NAck(void);				//IIC������ACK�ź�

void IIC3_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC3_Read_One_Byte(u8 daddr, u8 addr);

#endif

