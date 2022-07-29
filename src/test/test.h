// File: test.h

#ifndef __TEST_H
#define __TEST_H
#define TEST__USART_FLAG 0
#define TEST__FLASH_FLAG 0
#define TEST__SDRAM_FLAG 0
#define TEST__UC_MIN_FLAG 0
#define TEST__UC_MAX_FLAG 0
#define TEST__EC_MIN_FLAG 0
#define TEST__EC_MAX_FLAG 0
#define TEST__SOUND_FLAG 1

void set_debug_flag(uint8_t flag);
void set_debug_data(uint16_t data);
void test_manager(void);
#endif

// End of

