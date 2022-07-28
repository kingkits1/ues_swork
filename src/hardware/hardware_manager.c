// File: hardware_manager.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "../flash/w25qxx.h"
#include "../us_control/us_control.h"
#include "../es_control/es_control.h"
#include "../sdram/sdram.h"
extern TIM_HandleTypeDef htim7;
extern SDRAM_HandleTypeDef hsdram1;
void init_hardware_module(void)
{
    // USART
    // Flash(W25Q256)
    W25QXX1_Init();
    W25QXX2_Init();
    // TIMER
    HAL_TIM_Base_Start(&htim7);
    // SDRAM
    SDRAM_Initialization_Sequence(&hsdram1);
    
    // WM8978
    
    // FPGA
    
    // US control
    Start_US_work();
        
    // ES control
#if ES_FUNCTION
    Start_ES_work();
#endif    
}


// End of File:hardware_manager.c
