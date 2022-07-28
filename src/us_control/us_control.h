// File: us_control.h
#ifndef __US_CONTROL_H
#define __US_CONTROL_H
void US_Set_Pwr(uint16_t vol);
void US_Set_Freq(uint32_t freq);
void US_Set_Phase(uint8_t phase);
void Start_US_work(void);
void us_set_electric_relay(ENUM_ACTIVITY act);
void us_set_power_supply(ENUM_ACTIVITY act);
void us_set_output_enable(ENUM_ACTIVITY act);
void reset_us_control(void);
#endif

// End of file File: us_control.h
