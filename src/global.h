#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "sys/system.h"
#define CCMRAM       __attribute__((section(".ccmram")))
#define __ALIGNE(x)  __attribute__((aligned (x)))
typedef enum
{
    false,
    true
} bool;  //�������ͣ�ʹ��ǰ�����Զ���
void delay_us(uint32_t us);
#endif
