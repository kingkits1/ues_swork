#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
uint8_t wave_file_head[512]   __attribute__ ((at(0X2002AE00)));
uint8_t wave_play_buf1[4096]  __attribute__ ((at(0X2002B000)));
uint8_t wave_play_buf2[4096]  __attribute__ ((at(0X2002C000)));

