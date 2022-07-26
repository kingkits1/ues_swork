// File: global.c
#include "main.h"
#include "cmsis_os.h"
#include "global.h"
#include "stdio.h"
#include "key/key.h"
volatile uint32_t sys_ms_count;
volatile uint32_t sys_us_count;

__weak void empty_fun(void) {}
void short_delay(unsigned int count);
/*******************************************************************************
* 函 数 名         : delay_us
* 函数功能           : 延时函数，延时us
* 输    入         : i
* 输    出         : 无
* 独占SysTick资源
*******************************************************************************/
extern TIM_HandleTypeDef htim11;
void delay_us(uint32_t us)
{
#if 0
    uint32_t temp;
    SysTick->LOAD = 45 * us;     //设置重装数值, 72MHZ时
    SysTick->CTRL = 0X01;       //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL = 0;              //清零计数器
    do
    {
        temp = SysTick->CTRL;         //读取当前倒计数值
    }
    while((temp & 0x01) && (!(temp & (1 << 16)))); //等待时间到达
    SysTick->CTRL = 0;  //关闭计数器
    SysTick->VAL = 0;      //清空计数器
#else
    short_delay(us);
#endif
}

void short_delay(unsigned int count)
{
    // 延时16个时钟？？ n x 16 + 10
    while(count--)
    {
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
        __asm("\tnop");
    }
}


__weak void general_sys_reset(void)
{
    //register uint32_t R0; // Only for building warning
    //    __ASM("\tMOV R0, #1");
    //    __ASM("\tMSR FAULTMASK, R0");
    //    SCB->AIRCR = 0x05FA0004;
    //    for(;;);
    __DSB();															/* Ensure all outstanding memory accesses included
																	   buffered write are completed before reset */
    SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)	 |
                             (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                             SCB_AIRCR_SYSRESETREQ_Msk	 ); 		/* Keep priority group unchanged */
    __DSB();															/* Ensure completion of memory access */

    for(;;)															/* wait until reset */
    {
        __NOP();
    }
}

void init_global_data(void)
{
    // gpio
    init_key_val();
    // USART
    // TIMER
    sys_ms_count = 0;
    // SDRAM
    // ad9851
    // Flash(W25Q256)
    // WM8978
    // FPGA
    // US control
    // ES control
}



