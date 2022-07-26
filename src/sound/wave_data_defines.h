// File:wave_data_defines.h
#ifndef __WAVE_DATA_DEFINES_H
#define __WAVE_DATA_DEFINES_H
#define __ENABLE_WAVE_DATA_DOWNLOAD 0
// �ⲿFLASH WAV����
#define FLASH_EX_POWERUP_ADDR 0x30000
#define FLASH_EX_POWERDOWN_ADDR 0x80000
#define FLASH_EX_ALARM_ADDR 0x20000
//#define LENTH_OF_ALARM 40960
//#define LENTH_OF_START 282624
//#define LENTH_OF_SHUTDOWN 282624
#if __ENABLE_WAVE_DATA_DOWNLOAD
extern const unsigned char wave_data_us_max[];
extern const unsigned char wave_data_us_min[];
extern const unsigned char wave_data_us_close[];
extern const unsigned char wave_data_key_click[];
#else
#define wave_data_us_max    ((const unsigned char *)0x8100000)
#define wave_data_us_min    ((const unsigned char *)0x8110000)
#define wave_data_us_close  ((const unsigned char *)0x8120000)
#define wave_data_key_click ((const unsigned char *)0x81B0000)
#endif
typedef enum
{
    // US
    EM_WAV_US_CUTTING,    // US MAX �и�
    EM_WAV_US_HEMOSTASIS, // US MIN ֹѪ hemostasis
    EM_WAV_US_CLOSING,    // US CLOSING ǯ�ڱպ�
    // ALARM
    EM_WAV_ALARM_LOW,     // �����ȼ���ʾ��
    EM_WAV_ALARM_HIGH,    // �����ȼ���ʾ��
    // ES
    EM_WAV_ES_CUTTING,    // ES MAX �и�
    EM_WAV_ES_HEMOSTASIS, // ES MIN ֹѪ hemostasis
    EM_WAV_ES_CLOSING,    // ES CLOSING �պ�
    // US+ES
    EM_WAV_UES_CUTTING,    // US+ES MAX �и�
    EM_WAV_UES_HEMOSTASIS, // US+ES MIN ֹѪ hemostasis
    EM_WAV_UES_CLOSING,    // US+ES CLOSING �պ�
    // KEY pushed voice
    EM_WAV_KEY_CLICK,
    // POWER ON
    EM_WAV_POWER_ON,
    // SHUT DOWN
    EM_WAV_SHUT_DOWN,
    // OTHER
    EM_WAV_DEFAULT
    // TODO:
} ENUM_WAVE_TYPES;
#endif
// End of File:wave_data_defines.h
