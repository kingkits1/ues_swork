#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "sys/system.h"
#define CCMRAM       __attribute__((section(".ccmram")))
#define __ALIGNE(x)  __attribute__((aligned (x)))
typedef enum
{
    false,
    true
} bool;  //布尔类型，使用前必须自定义
void delay_us(uint32_t us);
#endif
