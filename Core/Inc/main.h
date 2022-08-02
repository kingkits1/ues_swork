/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BK1_MCU_FPGA_Pin GPIO_PIN_8
#define BK1_MCU_FPGA_GPIO_Port GPIOB
#define ESADJ_CTRL_Pin GPIO_PIN_5
#define ESADJ_CTRL_GPIO_Port GPIOB
#define BK2_MCU_FPGA_Pin GPIO_PIN_9
#define BK2_MCU_FPGA_GPIO_Port GPIOB
#define FMC_NL_NADV_Pin GPIO_PIN_7
#define FMC_NL_NADV_GPIO_Port GPIOB
#define ES_FTMIN_Pin GPIO_PIN_6
#define ES_FTMIN_GPIO_Port GPIOB
#define PWR_SUPV_Pin GPIO_PIN_13
#define PWR_SUPV_GPIO_Port GPIOJ
#define USMOS_EN_Pin GPIO_PIN_12
#define USMOS_EN_GPIO_Port GPIOJ
#define FMC_GPIO2_Pin GPIO_PIN_8
#define FMC_GPIO2_GPIO_Port GPIOI
#define ES_FQUD_Pin GPIO_PIN_4
#define ES_FQUD_GPIO_Port GPIOI
#define BK4_MCU_FPGA_Pin GPIO_PIN_7
#define BK4_MCU_FPGA_GPIO_Port GPIOK
#define BK3_MCU_FPGA_Pin GPIO_PIN_6
#define BK3_MCU_FPGA_GPIO_Port GPIOK
#define US_DDSRST_Pin GPIO_PIN_5
#define US_DDSRST_GPIO_Port GPIOK
#define ES_SPI6_MISO_OE_Pin GPIO_PIN_10
#define ES_SPI6_MISO_OE_GPIO_Port GPIOG
#define USADJ_ctrl_Pin GPIO_PIN_14
#define USADJ_ctrl_GPIO_Port GPIOJ
#define TS_YLED_Pin GPIO_PIN_2
#define TS_YLED_GPIO_Port GPIOI
#define ES_DDSRST_Pin GPIO_PIN_13
#define ES_DDSRST_GPIO_Port GPIOC
#define TS_BLED_Pin GPIO_PIN_5
#define TS_BLED_GPIO_Port GPIOI
#define FMC_GPIO1_Pin GPIO_PIN_7
#define FMC_GPIO1_GPIO_Port GPIOI
#define FMC_GPIO4_Pin GPIO_PIN_10
#define FMC_GPIO4_GPIO_Port GPIOI
#define SPI2_NSS_ES_DAC_Pin GPIO_PIN_6
#define SPI2_NSS_ES_DAC_GPIO_Port GPIOI
#define ES_MCUSpare2_Pin GPIO_PIN_4
#define ES_MCUSpare2_GPIO_Port GPIOK
#define ES_MCUSpare1_Pin GPIO_PIN_3
#define ES_MCUSpare1_GPIO_Port GPIOK
#define US_MCUSpare3_Pin GPIO_PIN_15
#define US_MCUSpare3_GPIO_Port GPIOJ
#define UniDetect_EN_Pin GPIO_PIN_2
#define UniDetect_EN_GPIO_Port GPIOD
#define TS_Touch_Pin GPIO_PIN_15
#define TS_Touch_GPIO_Port GPIOH
#define US_RelayCtrl_Pin GPIO_PIN_12
#define US_RelayCtrl_GPIO_Port GPIOI
#define FMC_GPIO3_Pin GPIO_PIN_9
#define FMC_GPIO3_GPIO_Port GPIOI
#define MCU_EEPROM_Pin GPIO_PIN_13
#define MCU_EEPROM_GPIO_Port GPIOH
#define ES_MOD_SW_Pin GPIO_PIN_14
#define ES_MOD_SW_GPIO_Port GPIOH
#define SPI2_NSS_US_DAC_Pin GPIO_PIN_0
#define SPI2_NSS_US_DAC_GPIO_Port GPIOI
#define FMC_GPIO5_Pin GPIO_PIN_11
#define FMC_GPIO5_GPIO_Port GPIOI
#define ES_MCUSpare3_Pin GPIO_PIN_1
#define ES_MCUSpare3_GPIO_Port GPIOK
#define ES_MCUSpare4_Pin GPIO_PIN_2
#define ES_MCUSpare4_GPIO_Port GPIOK
#define US_SPI6_MISO_OE_Pin GPIO_PIN_13
#define US_SPI6_MISO_OE_GPIO_Port GPIOI
#define Uni_CutBtn_Pin GPIO_PIN_15
#define Uni_CutBtn_GPIO_Port GPIOI
#define US_FQUD_Pin GPIO_PIN_11
#define US_FQUD_GPIO_Port GPIOJ
#define US_MCUSpare4_Pin GPIO_PIN_0
#define US_MCUSpare4_GPIO_Port GPIOK
#define BAT_CTRL_Pin GPIO_PIN_8
#define BAT_CTRL_GPIO_Port GPIOC
#define MCU_ErrorLED_Pin GPIO_PIN_7
#define MCU_ErrorLED_GPIO_Port GPIOC
#define BiDetect_EN_Pin GPIO_PIN_14
#define BiDetect_EN_GPIO_Port GPIOI
#define NegPlate_InsDect_Pin GPIO_PIN_8
#define NegPlate_InsDect_GPIO_Port GPIOJ
#define ES_OVERCurrent_Pin GPIO_PIN_10
#define ES_OVERCurrent_GPIO_Port GPIOJ
#define MCU_LED_Pin GPIO_PIN_6
#define MCU_LED_GPIO_Port GPIOC
#define US_MCUSpare2_Pin GPIO_PIN_3
#define US_MCUSpare2_GPIO_Port GPIOH
#define PZT_In_Dect_Pin GPIO_PIN_7
#define PZT_In_Dect_GPIO_Port GPIOJ
#define US_OVERCurren_Pin GPIO_PIN_9
#define US_OVERCurren_GPIO_Port GPIOJ
#define SPI6_NSS_ES_Pin GPIO_PIN_7
#define SPI6_NSS_ES_GPIO_Port GPIOG
#define SPI6_NSS_US_Pin GPIO_PIN_6
#define SPI6_NSS_US_GPIO_Port GPIOG
#define F_CS1_Pin GPIO_PIN_6
#define F_CS1_GPIO_Port GPIOF
#define US_MCUSpare1_Pin GPIO_PIN_2
#define US_MCUSpare1_GPIO_Port GPIOH
#define ES_FTEN_Pin GPIO_PIN_6
#define ES_FTEN_GPIO_Port GPIOJ
#define F_CS2_Pin GPIO_PIN_10
#define F_CS2_GPIO_Port GPIOF
#define BK_ESADJ_SW_Pin GPIO_PIN_4
#define BK_ESADJ_SW_GPIO_Port GPIOJ
#define ESMOS_EN_Pin GPIO_PIN_12
#define ESMOS_EN_GPIO_Port GPIOD
#define SPI6_CS_PG3_ES_Pin GPIO_PIN_3
#define SPI6_CS_PG3_ES_GPIO_Port GPIOG
#define ES_HDLctrl_Pin GPIO_PIN_5
#define ES_HDLctrl_GPIO_Port GPIOJ
#define US_OUT_SW_Pin GPIO_PIN_12
#define US_OUT_SW_GPIO_Port GPIOH
#define FAN_ctr_Pin GPIO_PIN_3
#define FAN_ctr_GPIO_Port GPIOJ
#define ETH_RESET_Pin GPIO_PIN_11
#define ETH_RESET_GPIO_Port GPIOD
#define Uni_InDect_Pin GPIO_PIN_7
#define Uni_InDect_GPIO_Port GPIOH
#define ES_FTMAX_Pin GPIO_PIN_9
#define ES_FTMAX_GPIO_Port GPIOH
#define ESPZT_CNT_Pin GPIO_PIN_11
#define ESPZT_CNT_GPIO_Port GPIOH
#define ES_ADJ_FDBK_Pin GPIO_PIN_6
#define ES_ADJ_FDBK_GPIO_Port GPIOA
#define FT_MIN_Pin GPIO_PIN_2
#define FT_MIN_GPIO_Port GPIOJ
#define ESPZT_InDect_Pin GPIO_PIN_6
#define ESPZT_InDect_GPIO_Port GPIOH
#define PZT_CNT_Pin GPIO_PIN_8
#define PZT_CNT_GPIO_Port GPIOH
#define US_LM5085_SW_Pin GPIO_PIN_10
#define US_LM5085_SW_GPIO_Port GPIOH
#define US_ADJ_FDBK_Pin GPIO_PIN_3
#define US_ADJ_FDBK_GPIO_Port GPIOA
#define PowerSupplyV_Pin GPIO_PIN_1
#define PowerSupplyV_GPIO_Port GPIOB
#define LCD_BL_Ctrl_Pin GPIO_PIN_0
#define LCD_BL_Ctrl_GPIO_Port GPIOJ
#define FT_EN_Pin GPIO_PIN_1
#define FT_EN_GPIO_Port GPIOJ
#define FT_MAX_Pin GPIO_PIN_14
#define FT_MAX_GPIO_Port GPIOB
#define HDL_CTRL_Pin GPIO_PIN_15
#define HDL_CTRL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
