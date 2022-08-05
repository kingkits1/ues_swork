// File gpio.h
#ifndef __GPIO_H
#define __GPIO_H
#define SET_POWER_ON_LED() {PIout(5)=1;PIout(2)=0;}
#define SET_POWER_ON_LED() {PIout(5)=0;PIout(2)=1;}
#endif

// End of File gpio.h

