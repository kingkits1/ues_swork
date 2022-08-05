#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
uint8_t wave_file_head[512];
uint8_t wave_play_buf1[4096];
uint8_t wave_play_buf2[4096];
uint8_t wav_flash_temp_buf[4096];

