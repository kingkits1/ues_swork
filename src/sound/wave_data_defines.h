// File:wave_data_defines.h
#ifndef __WAVE_DATA_DEFINES_H
#define __WAVE_DATA_DEFINES_H
#define __ENABLE_WAVE_DATA_DOWNLOAD 0
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
    EM_WAV_US_CUTTING,    // US MAX 切割
    EM_WAV_US_HEMOSTASIS, // US MIN 止血 hemostasis
    EM_WAV_US_CLOSING,    // US CLOSING 钳口闭合
    // ALARM
    EM_WAV_ALARM_LOW,     // 低优先级警示音
    EM_WAV_ALARM_HIGH,    // 高优先级警示音
    // ES
    EM_WAV_ES_CUTTING,    // ES MAX 切割
    EM_WAV_ES_HEMOSTASIS, // ES MIN 止血 hemostasis
    EM_WAV_ES_CLOSING,    // ES CLOSING 闭合
    // US+ES
    EM_WAV_UES_CUTTING,    // US+ES MAX 切割
    EM_WAV_UES_HEMOSTASIS, // US+ES MIN 止血 hemostasis
    EM_WAV_UES_CLOSING,    // US+ES CLOSING 闭合
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
