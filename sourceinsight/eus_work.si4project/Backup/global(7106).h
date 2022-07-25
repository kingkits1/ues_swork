#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "sys/system.h"

// Function defines
#define USE_HAL_I2C_FUNCTION 1 // �����Ƿ�ʹ��Ӳ����I2C


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
} bool;  //�������ͣ�ʹ��ǰ�����Զ���


// ������Ժ���
extern uint8_t debug_printf_flag; // �˱�����debug.c����
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
