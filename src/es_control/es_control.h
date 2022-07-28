// File: es_control.h
#ifndef __ES_CONTROL_H
#define __ES_CONTROL_H
void ES_Set_Pwr(uint16_t vol);
void ES_Set_Freq(uint32_t freq);
void ES_Set_Phase(uint8_t phase);
void Start_ES_work(void);
void es_set_electric_relay(ENUM_ACTIVITY act);
void es_set_power_supply(ENUM_ACTIVITY act);

void reset_es_control(void);

#endif

// End of file File: es_control.h
