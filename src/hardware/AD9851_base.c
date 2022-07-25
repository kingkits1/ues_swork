// File: AD9851_base.c
#include "AD9851_base.h"
#include "../global.h"
extern SPI_HandleTypeDef hspi3;
UN_ES_US_9851_DATA es_9851_data;
UN_ES_US_9851_DATA us_9851_data;
void ad9851_wr_data(UN_ES_US_9851_DATA *data)
{
    HAL_SPI_Transmit(&hspi3, (uint8_t *)(data->byte), 5, 1000);
}

void init_ad9851_data(UN_ES_US_9851_DATA *data, uint32_t freq)
{
    data->base.set_phase.REFCLKx6_enable = 1;
    data->base.set_phase.const_bit = 0;
    data->base.set_phase.powerdown_bit = 0;
    data->base.set_phase.phase = 1; // 11.25
    data->base.set_frequency = freq;
}

void ad9851_set_freq(UN_ES_US_9851_DATA *data, uint32_t freq)
{
    data->base.set_frequency = freq;
}
void ad9851_set_phase(UN_ES_US_9851_DATA *data, uint8_t phase)
{
    data->base.set_phase.phase = phase;
}


#define ES_UPDATE_PIN PIout(4)
#define ES_RESET_PIN PCout(13)
#define US_RESET_PIN PKout(5)
#define US_UPDATE_PIN PJout(11)
void es_update_ad9851_freq_data(void)
{
    ES_UPDATE_PIN = 0;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    ES_UPDATE_PIN = 1;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    ES_UPDATE_PIN = 0;
}
void es_reset_ad9851_chip(void)
{
    ES_RESET_PIN = 0;
    delay_us(2);
    ES_RESET_PIN = 1;
    delay_us(2);
    ES_RESET_PIN = 0;
    delay_us(10);
}


void us_update_ad9851_freq_data(void)
{
    US_UPDATE_PIN = 0;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    US_UPDATE_PIN = 1;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    US_UPDATE_PIN = 0;
}
void us_reset_ad9851_chip(void)
{
    US_RESET_PIN = 0;
    delay_us(2);
    US_RESET_PIN = 1;
    delay_us(2);
    US_RESET_PIN = 0;
    delay_us(10);
}

