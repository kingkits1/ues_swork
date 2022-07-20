#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

#define SDA3_IN()  {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	//PH5����ģʽ
#define SDA3_OUT() {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} //PH5���ģʽ

#define IIC3_SCL   PAout(8) //SCL     //kangyingkui 2021.03.10
#define IIC3_SDA   PCout(9) //SDA
#define READ_SDA3  PCin(9)  //����SDA

//IIC���в�������
void IIC3_Init(void);                //��ʼ��IIC��IO��
void IIC3_Start(void);				//����IIC��ʼ�ź�
void IIC3_Stop(void);	  			//����IICֹͣ�ź�
void IIC3_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC3_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC3_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC3_Ack(void);					//IIC����ACK�ź�
void IIC3_NAck(void);				//IIC������ACK�ź�

void IIC3_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC3_Read_One_Byte(uint8_t daddr, uint8_t addr);

#endif

