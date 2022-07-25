// File audioplay.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "wave_data_defines.h"
#include "wavplay.h"
#include "../hardware/sai.h"
#include "../hardware/wm8978.h"
#include "../malloc/mem_defines.h"

#include "audioplay.h"
static UNION_AUDIO_DATA play_data;

void clear_play_data(void)
{
    play_data.all = 0;
}

void play_stop_wav(void)
{
    // Stop DMA
    SAI_Play_Stop();
    play_data.all = 0;
    set_wav_control_active_flag(false);
}

void play_start_wav(ENUM_WAVE_TYPES wav_type)
{
    // 先清除标记
    play_data.all = 0;
    play_data.bits.wav_types = (uint32_t) wav_type;
    // 再根据需要设置标记
    switch(wav_type)
    {
    // US
    case EM_WAV_US_CUTTING:   // US MAX 切割
        play_data.bits.wav_us_cutting = 1;
        break;
    case EM_WAV_US_HEMOSTASIS: // US MIN 止血 hemostasis
        play_data.bits.wav_us_hemostasis = 1;
        break;
    case EM_WAV_US_CLOSING:    // US CLOSING 钳口闭合
        play_data.bits.wav_us_closing = 1;
        break;
    // ALARM
    case EM_WAV_ALARM_LOW:     // 低优先级警示音
        play_data.bits.wav_alarm_low = 1;
        break;
    case EM_WAV_ALARM_HIGH:    // 高优先级警示音
        play_data.bits.wav_alarm_high = 1;
        break;
    // ES
    case EM_WAV_ES_CUTTING:    // ES MAX 切割
        play_data.bits.wav_es_cutting = 1;
        break;
    case EM_WAV_ES_HEMOSTASIS: // ES MIN 止血 hemostasis
        play_data.bits.wav_es_hemostasis = 1;
        break;
    case EM_WAV_ES_CLOSING:    // ES CLOSING 闭合
        play_data.bits.wav_es_closing = 1;
        break;
    // US+ES
    case EM_WAV_UES_CUTTING:   // US+ES MAX 切割
        play_data.bits.wav_ues_cutting = 1;
        break;
    case EM_WAV_UES_HEMOSTASIS: // US+ES MIN 止血 hemostasis
        play_data.bits.wav_ues_hemostasis = 1;
        break;
    case EM_WAV_UES_CLOSING:    // US+ES CLOSING 闭合
        play_data.bits.wav_ues_closing = 1;
        break;
    // KEY pushed voice
    case EM_WAV_KEY_CLICK:
        play_data.bits.wav_key_click = 1;
        break;
    // POWER ON
    case EM_WAV_POWER_ON:
        play_data.bits.wav_power_on = 1;
        break;
    // SHUT DOWN
    case EM_WAV_SHUT_DOWN:
        play_data.bits.wav_shut_down = 1;
        break;
    // OTHER
    case EM_WAV_DEFAULT:
        break;
    default:
        break;  // TODO:
    }
}


void wav_play_manager(void)
{
    if(play_data.all == 0) return; // 没有声音播放需求
    // 下面处理数据
    wav_play_song((ENUM_WAVE_TYPES)play_data.bits.wav_types);

    play_data.all = 0; // 清除标记防止重入
}

// End of File:audioplay.c

