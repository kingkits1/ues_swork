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
* �� �� ��         : delay_us
* ��������           : ��ʱ��������ʱus
* ��    ��         : i
* ��    ��         : ��
* ��ռSysTick��Դ
*******************************************************************************/
extern TIM_HandleTypeDef htim11;
void delay_us(uint32_t us)
{
#if 0
    uint32_t temp;
    SysTick->LOAD = 45 * us;     //������װ��ֵ, 72MHZʱ
    SysTick->CTRL = 0X01;       //ʹ�ܣ����������޶����������ⲿʱ��Դ
    SysTick->VAL = 0;              //���������
    do
    {
        temp = SysTick->CTRL;         //��ȡ��ǰ������ֵ
    }
    while((temp & 0x01) && (!(temp & (1 << 16)))); //�ȴ�ʱ�䵽��
    SysTick->CTRL = 0;  //�رռ�����
    SysTick->VAL = 0;      //��ռ�����
#else
    short_delay(us);
#endif
}

void short_delay(unsigned int count)
{
    // ��ʱ16��ʱ�ӣ��� n x 16 + 10
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



