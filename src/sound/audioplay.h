// File:audioplay.h
#ifndef __AUDIOPLAY_H
#define __AUDIOPLAY_H
typedef struct
{
    // US
    uint32_t wav_us_cutting: 1;    //EM_WAV_US_CUTTING, // US MAX 切割
    uint32_t wav_us_hemostasis: 1; //EM_WAV_US_HEMOSTASIS, // US MIN 止血 hemostasis
    uint32_t wav_us_closing: 1;    //EM_WAV_US_CLOSING,    // US CLOSING 钳口闭合
    // ALARM
    uint32_t wav_alarm_low: 1;     //EM_WAV_ALARM_LOW,     // 低优先级警示音
    uint32_t wav_alarm_high: 1;    //EM_WAV_ALARM_HIGH,    // 高优先级警示音
    // ES
    uint32_t wav_es_cutting: 1;    //EM_WAV_ES_CUTTING,    // ES MAX 切割
    uint32_t wav_es_hemostasis: 1; //EM_WAV_ES_HEMOSTASIS, // ES MIN 止血 hemostasis
    uint32_t wav_es_closing: 1;    //EM_WAV_ES_CLOSING,    // ES CLOSING 闭合
    // US+ES
    uint32_t wav_ues_cutting: 1;   //EM_WAV_UES_CUTTING,    // US+ES MAX 切割
    uint32_t wav_ues_hemostasis: 1; //EM_WAV_UES_HEMOSTASIS, // US+ES MIN 止血 hemostasis
    uint32_t wav_ues_closing: 1;   //EM_WAV_UES_CLOSING,    // US+ES CLOSING 闭合
    // KEY pushed voice
    uint32_t wav_key_click: 1;     //EM_WAV_KEY_CLICK,
    // POWER ON
    uint32_t wav_power_on: 1;      //EM_WAV_POWER_ON,
    // SHUT DOWN
    uint32_t wav_shut_down: 1;      //EM_WAV_SHUT_DOWN,
    // OTHER
    uint32_t unused: 2; //EM_WAV_DEFAULT
    uint32_t wav_types: 16;
    // TODO:
} ST_AUDIO_DATA;

typedef union
{
    ST_AUDIO_DATA bits;
    uint32_t all;
} UNION_AUDIO_DATA;


///
void play_stop_wav(void);
void play_start_wav(ENUM_WAVE_TYPES wav_type);
void wav_play_manager(void);
// Input: val 设置的音量 0-10
void set_volumn(uint8_t scale);
#endif

// End of File:audioplay.h

