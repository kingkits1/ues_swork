#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "sys/system.h"

// Function defines
#define USE_HAL_I2C_FUNCTION 1 // 定义是否使用硬件的I2C


#define CCMRAM       __attribute__((section(".ccmram")))
#define __ALIGNE(x)  __attribute__((aligned (x)))
typedef enum
{
    EM_SUCCEED,
    EM_FAIL
} ENUM_OPERATION_RESULT_DEFINES;


typedef enum
{
    EM_DISABLE = 0,
    EM_ENABLE = 1
} ENUM_ACTIVITY;
typedef enum
{
    false,
    true
} bool;  //布尔类型，使用前必须自定义


// 定义调试函数
extern uint8_t debug_printf_flag; // 此变量由debug.c定义
#ifdef DEBUG
#define dbg_printf(fmt, args...) \
	{                            \
		debug_printf_flag = 1;	 \
		printf("<D>");           \
		printf(fmt, ##args);     \
		debug_printf_flag = 0;   \
	}
#else
#define dbg_printf(fmt, args...)
#endif

void empty_fun(void);
void delay_us(uint32_t us);
#endif
