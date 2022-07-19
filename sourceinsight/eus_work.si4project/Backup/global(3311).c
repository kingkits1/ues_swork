// File: global.c
#include "main.h"
#include "cmsis_os.h"
#include "global.h"
/*******************************************************************************
* �� �� ��         : delay_us
* ��������           : ��ʱ��������ʱus
* ��    ��         : i
* ��    ��         : ��
* ��ռSysTick��Դ
*******************************************************************************/
void delay_us(uint32_t us)
{
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
}

void short_delay(unsigned int count)
{
    while(count--)
    {
        __asm("\tnop"); __asm("\tnop"); __asm("\tnop"); __asm("\tnop"); __asm("\tnop");	__asm("\tnop");	__asm("\tnop");	__asm("\tnop");
        __asm("\tnop"); __asm("\tnop"); __asm("\tnop"); __asm("\tnop"); __asm("\tnop");	__asm("\tnop");	__asm("\tnop");	__asm("\tnop");
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
