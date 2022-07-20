#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

#define SDA3_IN()  {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	//PH5输入模式
#define SDA3_OUT() {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} //PH5输出模式

#define IIC3_SCL   PAout(8) //SCL     //kangyingkui 2021.03.10
#define IIC3_SDA   PCout(9) //SDA
#define READ_SDA3  PCin(9)  //输入SDA

//IIC所有操作函数
void IIC3_Init(void);                //初始化IIC的IO口
void IIC3_Start(void);				//发送IIC开始信号
void IIC3_Stop(void);	  			//发送IIC停止信号
void IIC3_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC3_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC3_Wait_Ack(void); 				//IIC等待ACK信号
void IIC3_Ack(void);					//IIC发送ACK信号
void IIC3_NAck(void);				//IIC不发送ACK信号

void IIC3_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC3_Read_One_Byte(uint8_t daddr, uint8_t addr);

#endif

