// File: AD9851_base.h
#ifndef __AD9851_BASE_H
#define __AD9851_BASE_H
#include "stm32f4xx.h"
typedef enum 
{
    EM_ES_WORK_DATA,
    EM_US_WORK_DATA
}ENUM_WORK_DATA_TYPE;


typedef struct
{
    uint8_t REFCLKx6_enable :1;
    uint8_t const_bit       :1;
    uint8_t powerdown_bit   :1;
    uint8_t phase           :5;
}ST_AD9851_PHASE_BYTE;

typedef struct
{
    uint32_t set_frequency;
    ST_AD9851_PHASE_BYTE set_phase;
}ST_9851_BASE;

typedef union
{
   ST_9851_BASE base;
   uint8_t byte[5];
}UN_ES_US_9851_DATA;
extern UN_ES_US_9851_DATA es_9851_data;
extern UN_ES_US_9851_DATA us_9851_data;


void ad9851_wr_data(UN_ES_US_9851_DATA *data);
void init_ad9851_data(UN_ES_US_9851_DATA *data, uint32_t freq);
void ad9851_set_freq(UN_ES_US_9851_DATA *data, uint32_t freq);
void ad9851_set_phase(UN_ES_US_9851_DATA *data, uint8_t phase);
void es_update_ad9851_freq_data(void);
void es_reset_ad9851_chip(void);
void us_update_ad9851_freq_data(void);
void us_reset_ad9851_chip(void);
#endif
// End of file AD9851_base.h
