#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "stdio.h"
#include "test.h"
#include "../sound/wave_data_defines.h"
#include "../sound/audioplay.h"
CCMRAM static uint8_t debug_flag;
CCMRAM static uint16_t debug_data;

// 变量预定义 ---->
#if TEST__USART_FLAG
#endif

#if TEST__SDRAM_FLAG
void fpga_read_data(void);
void clear_fpga_data(void);
uint16_t FPGA_Read(uint8_t addr);
void set_fpga_read_port(bool act);

#endif

#if TEST__FLASH_FLAG
uint16_t W25QXX1_ReadID(void);
void W25QXX1_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void W25QXX1_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX1_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
uint16_t W25QXX2_ReadID(void);
void W25QXX2_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void W25QXX2_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX2_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

static uint16_t tstbuf[256];
static uint16_t tstrbuf[256];
static uint16_t flashid;
void test_flash1(void)
{
    int i;
    flashid = W25QXX1_ReadID();
    flashid = flashid;
    for(i = 0; i < 256; i++)
    {
        tstbuf[i] = i;
    }
    W25QXX1_Write_NoCheck((uint8_t *)tstbuf, 0, 512);
    W25QXX1_Read((uint8_t *)tstrbuf, 0, 512);
}
void test_flash2(void)
{
    int i;
    flashid = W25QXX2_ReadID();
    for(i = 0; i < 256; i++)
    {
        tstbuf[i] = 0x3000 - i;
    }
    W25QXX2_Write_NoCheck((uint8_t *)tstbuf, 0, 512);
    W25QXX2_Read((uint8_t *)tstrbuf, 0, 512);
}
#endif

#if TEST__UC_MIN_FLAG
#endif

#if TEST__UC_MAX_FLAG
#endif

#if TEST__EC_MIN_FLAG
#endif

#if TEST__EC_MAX_FLAG
#endif

#if TEST__SOUND_FLAG
void set_volumn(uint8_t scale);
void SAI_Play_Stop(void);
void play_start_wav(ENUM_WAVE_TYPES wav_type);
#endif


void set_debug_flag(uint8_t flag)
{
    debug_flag = flag;
}

void set_debug_data(uint16_t data)
{
    debug_data = data;
}

void init_test_data(void) {}
void test_manager(void)
{
#if TEST__FLASH_FLAG
    if(debug_flag == 1)
    {
        debug_flag = 0;
        test_flash1();
    }
    if(debug_flag == 2)
    {
        debug_flag = 0;
        test_flash2();
    }
#endif
#if TEST__USART_FLAG
    if(debug_flag == 3)
    {
        debug_flag = 0;
        printf("TEST\n");
    }
#endif
#if TEST__SDRAM_FLAG
    if(debug_flag == 4)
    {
        debug_flag = 0;
        clear_fpga_data();
        fpga_read_data();
    }

    if(debug_flag == 0x5)
    {
        debug_flag = 0;
    }
    if(debug_flag == 0x6)
    {
        debug_flag = 0;
        set_fpga_read_port(true);
        debug_data = FPGA_Read(0);
        debug_data = FPGA_Read(1);
        debug_data = FPGA_Read(2);
        debug_data = FPGA_Read(3);
        debug_data = FPGA_Read(4);
        debug_data = FPGA_Read(5);
        set_fpga_read_port(false);
    }
#endif

#if TEST__UC_MIN_FLAG
#endif
#if TEST__UC_MAX_FLAG
#endif

#if TEST__EC_MIN_FLAG
#endif

#if TEST__EC_MAX_FLAG
#endif

#if TEST__SOUND_FLAG
    if(debug_flag == 7)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_US_CUTTING);
    }
    if(debug_flag == 8)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_US_HEMOSTASIS);
    }
    if(debug_flag == 9)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_US_CLOSING);
    }
    if(debug_flag == 0xa)
    {
        debug_flag = 0;
        SAI_Play_Stop();
    }
    if(debug_flag == 0xb)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_KEY_CLICK);
    }
    if(debug_flag == 0xc)
    {
        debug_flag = 0;
        play_start_wav(EM_WAV_POWER_ON);
    }
    if(debug_flag == 0xd)
    {
        debug_flag = 0;
        debug_data = 2;
        set_volumn(debug_data);
    }

#endif
}


