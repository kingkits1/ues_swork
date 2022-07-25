// File:wave_data_defines.h
#ifndef __WAVE_DATA_DEFINES_H
#define __WAVE_DATA_DEFINES_H
extern unsigned char wave_data_us_max[];
extern unsigned char wave_data_us_min[];
extern unsigned char wave_data_us_close[];
extern unsigned char wave_data_key_click[];

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
