// File: i2c.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"

#if USE_HAL_I2C_FUNCTION
#include "i2c.h"

extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;
//==========
// º¯Êý¶¨Òå
//==========
bool is_i2c_busy(I2C_HandleTypeDef *hi2c)
{
    return (hi2c->Instance->SR2 & 0x2) ? true : false;
}


void I2C_force_reset_interface(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_DeInit(hi2c);
    HAL_I2C_MspDeInit(hi2c);
    HAL_Delay(5);
    HAL_I2C_MspInit(hi2c);
    HAL_I2C_Init(hi2c);
    HAL_Delay(5);
}


//PA8	I2C3_SCL	n/a Alternate Function Open Drain	Pull-up Very High	I2C2_SCL_BMP280 true
//PC9	I2C3_SDA	n/a Alternate Function Open Drain	Pull-up Very High	I2C2_SDA_BMP280 true
void reset_i2c3_master(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t status, count;

    HAL_I2C_DeInit(&hi2c3);
    __HAL_RCC_I2C3_CLK_DISABLE();
    /**I2C2 GPIO Configuration
    PA8     ------> I2C3_SCL
    PC9     ------> I2C3_SDA
    */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    GPIO_InitStruct.Pin = GPIO_PIN_8;    
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);//PHout(5) = 1;  // SDA = 1;
    delay_us(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);//PHout(4) = 1;  // SCL = 1;
    delay_us(100);

    status = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);
    count = 0;
    while(!status)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);//PHout(4) = 0;  // SCL = 0;
        delay_us(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);//PHout(4) = 1;  // SCL = 1;
        delay_us(100);
        status = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);//PCin(9);
        if(count++ > 16) break;
    }
    __HAL_RCC_I2C3_CLK_ENABLE();
    HAL_I2C_Init(&hi2c3);
    SET_BIT(hi2c3.Instance->CR1, I2C_CR1_SWRST);
    delay_us(300);
    CLEAR_BIT(hi2c3.Instance->CR1, I2C_CR1_SWRST);
    delay_us(300);
}

//PH4	I2C2_SCL	n/a Alternate Function Open Drain	Pull-up Very High	I2C2_SCL_BMP280 true
//PH5	I2C2_SDA	n/a Alternate Function Open Drain	Pull-up Very High	I2C2_SDA_BMP280 true
void reset_i2c2_master(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t status, count;

    HAL_I2C_DeInit(&hi2c2);
    __HAL_RCC_I2C2_CLK_DISABLE();
    /**I2C2 GPIO Configuration
    PH4     ------> I2C2_SCL
    PH5     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET);//PHout(5) = 1;  // SDA = 1;
    delay_us(100);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET);//PHout(4) = 1;  // SCL = 1;
    delay_us(100);

    status = HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5);//PHin(5);
    count = 0;
    while(!status)
    {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_RESET);//PHout(4) = 0;  // SCL = 0;
        delay_us(100);
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET);//PHout(4) = 1;  // SCL = 1;
        delay_us(100);
        status = HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5);//PHin(5);
        if(count++ > 16) break;
    }

    __HAL_RCC_I2C2_CLK_ENABLE();
    HAL_I2C_Init(&hi2c2);
    SET_BIT(hi2c2.Instance->CR1, I2C_CR1_SWRST);
    delay_us(300);
    CLEAR_BIT(hi2c2.Instance->CR1, I2C_CR1_SWRST);
    delay_us(300);
}
__weak void IIC3_Init(void){}

#else
#include "myiic3.c"

#endif

//end of File: i2c.c

