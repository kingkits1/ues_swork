#ifndef _SDRAM_H
#define _SDRAM_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
extern SDRAM_HandleTypeDef hsdram1;
#define Bank5_SDRAM_ADDR    ((uint32_t)(0XC0000000)) //SDRAM开始地址

//SDRAM配置参数
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

uint8_t SDRAM_Send_Cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval);
void FMC_SDRAM_WriteBuffer(uint16_t *pBuffer, uint32_t WriteAddr, uint32_t len);
void FMC_SDRAM_ReadBuffer(uint16_t *pBuffer, uint32_t ReadAddr, uint32_t len);
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);

#endif
