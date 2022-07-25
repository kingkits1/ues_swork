// File: test.h

#ifndef __TEST_H
#define __TEST_H

#define EM_TEST_SDRAM_FLAG 0
#define EM_TEST_UC_MIN_FLAG 0
#define EM_TEST_UC_MAX_FLAG 0
#define EM_TEST_EC_MIN_FLAG 0
#define EM_TEST_EC_MAX_FLAG 0
#define EM_TEST_SOUND_FLAG 0

void set_debug_flag(uint8_t flag);
void set_debug_data(uint16_t data);
void test_manager(void);
#endif

// End of 

