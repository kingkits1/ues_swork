#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "wave_data_defines.h"
#include "wavplay.h"
#include "../hardware/sai.h"
#include "../hardware/wm8978.h"
#include "../malloc/mem_defines.h"
//////////////////////////////////////////////////////////////////////////////////
//1,支持16位/24位WAV文件播放
//2,最高可以支持到192K/24bit的WAV格式.
//////////////////////////////////////////////////////////////////////////////////

////////
__wavctrl wavctrl;		//WAV控制结构体
volatile uint8_t wavtransferend = 0;	//sai传输完成标志
volatile uint8_t wavwitchbuf = 0;		//saibufx指示标志


//WAV解析初始化
//wavx:wav 信息存放结构体指针
//返回值:0,成功;1,打开文件失败;2,非WAV文件;3,DATA区域未找到.
uint8_t wav_decode_init(ENUM_WAVE_TYPES wav_type)
{
    uint8_t *buf;
    //ChunkRIFF *riff;
    ChunkFMT *fmt;
    ChunkFACT *fact;
    ChunkDATA *data;

    wavctrl.actived_flag = false;
    switch(wav_type)
    {
    // US
    case EM_WAV_US_CUTTING:   // US MAX 切割
        buf = wave_data_us_max;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    case EM_WAV_US_HEMOSTASIS: // US MIN 止血 hemostasis
        buf = wave_data_us_min;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    case EM_WAV_US_CLOSING:    // US CLOSING 钳口闭合
        buf = wave_data_us_close;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    // ALARM
    case EM_WAV_ALARM_LOW:     // 低优先级警示音
        break;
    case EM_WAV_ALARM_HIGH:    // 高优先级警示音
        break;
    // ES
    case EM_WAV_ES_CUTTING:    // ES MAX 切割
        break;
    case EM_WAV_ES_HEMOSTASIS: // ES MIN 止血 hemostasis
        break;
    case EM_WAV_ES_CLOSING:    // ES CLOSING 闭合
        break;
    // US+ES
    case EM_WAV_UES_CUTTING:   // US+ES MAX 切割
        break;
    case EM_WAV_UES_HEMOSTASIS: // US+ES MIN 止血 hemostasis
        break;
    case EM_WAV_UES_CLOSING:    // US+ES CLOSING 闭合
        break;
    // KEY pushed voice
    case EM_WAV_KEY_CLICK:
        buf = wave_data_key_click;
        wavctrl.loop_sound_flag = false;
        wavctrl.actived_flag = true;
        break;
    // POWER ON
    case EM_WAV_POWER_ON:
        break;
    // SHUT DOWN
    case EM_WAV_SHUT_DOWN:
        break;
    // OTHER
    case EM_WAV_DEFAULT:
        break;
    default:
        break;  // TODO:
    }
    // 防止错误输出
    if(buf == wave_file_head) return 1;

    //riff=(ChunkRIFF *)buf;		//获取RIFF块
    //if(riff->Format==0X45564157)//是WAV文件
    //{
    fmt = (ChunkFMT *)(buf + 12);	//获取FMT块
    fact = (ChunkFACT *)(buf + 12 + 8 + fmt->ChunkSize); //读取FACT块
    if(fact->ChunkID == 0X74636166 || fact->ChunkID == 0X5453494C)
    {
        wavctrl.datalocate = 12 + 8 + fmt->ChunkSize + 8 + fact->ChunkSize; //具有fact/LIST块的时候(未测试)
    }
    else
    {
        wavctrl.datalocate = 12 + 8 + fmt->ChunkSize;
    }


    data = (ChunkDATA *)(buf + wavctrl.datalocate);	//读取DATA块
    //if(data->ChunkID==0X61746164)//解析成功!
    //{
    wavctrl.audioformat = fmt->AudioFormat;		//音频格式
    wavctrl.nchannels = fmt->NumOfChannels;		//通道数
    wavctrl.samplerate = fmt->SampleRate;		//采样率
    wavctrl.bitrate = fmt->ByteRate * 8;			//得到位速
    wavctrl.blockalign = fmt->BlockAlign;		//块对齐
    wavctrl.bps = fmt->BitsPerSample;			//位数,16/24/32位

    wavctrl.datasize = data->ChunkSize;			//数据块大小
    wavctrl.datalocate = wavctrl.datalocate + 8;		//数据流开始的地方.
    //}
    //}
    // zzx 初始化起始数据
    wavctrl.datastart = buf + wavctrl.datalocate;
    wavctrl.current_data = wavctrl.datastart;
    wavctrl.data_end = wavctrl.datastart + wavctrl.datasize;
    return 0;
}

//填充buf
//buf:数据区
//size:填充数据量.zzx ！必须为4的倍数
//bits:位数(16/24)
//返回值:读到的数据个数
uint16_t wav_buffill(uint8_t *buf, uint16_t size)
{
    uint16_t readlen = 0;
    uint8_t *pbuf;
    uint8_t count;
    // 如果音频数据未准备好，则直接返回！！！
    if(wavctrl.actived_flag == false)	return 0;

    pbuf = buf;
    if(wavctrl.bps == 24) //24bit音频,需要处理一下
    {
        // zzx:24bit时，实际存储上每3个字节为一个数据，
        // 到缓冲区时，需要写入4字节（最高字节的数据
        // 是无效的），这里是3字节转4字节！
        // 而且需要处理数据不够一个缓冲区时的问题！！！
        // 这时要考虑是否为循环数据！！！！

        count = 0;
        while(readlen < size)
        {
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            if(count >= 3)
            {
                // 填充的数据
                count = 0;
                *pbuf = 0;
                readlen ++;
            }
            if(wavctrl.current_data >= wavctrl.data_end)
            {
                if(wavctrl.loop_sound_flag == true)
                {
                    wavctrl.current_data = wavctrl.datastart;
                }
                else
                {
                    // 填充0，并返回播放完成
                    while(readlen++ < size)
                    {
                        *pbuf = 0;
                        pbuf++;
                        // TODO: !!!!// 设置停止标志
                        if(wavwitchbuf == 0)
                        {
                            wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END0;
                        }
                        else
                        {
                            wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END1;
                        }
                        return readlen;
                    }
                }
            }
        }
    }
    else
    {
        while(readlen < size)
        {
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            if(wavctrl.current_data >= wavctrl.data_end)
            {
                if(wavctrl.loop_sound_flag == true)
                {
                    wavctrl.current_data = wavctrl.datastart;
                }
                else
                {
                    // 填充0，并返回播放完成
                    while(readlen++ < size)
                    {
                        *pbuf = 0;
                        pbuf++;
                        //TODO: !!!!// 设置停止标志
                        if(wavwitchbuf == 0)
                        {
                            wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END0;
                        }
                        else
                        {
                            wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END1;
                        }
                        return readlen;
                    }
                }
            }
        }
    }
    return readlen;
}
//WAV播放时,SAI DMA传输回调函数
void wav_sai_dma_tx_callback(void)
{
    if(DMA2_Stream3->CR & (1 << 19))
    {
        wavwitchbuf = 0;
        if(wavctrl.stop_buf_flag != EM_WAV_TWO_BUFFER_END0)
        {
            wav_buffill(wave_play_buf1, 4096);
        }
        else
        {
            // Stop DMA
            SAI_Play_Stop();
            wavctrl.actived_flag = false;
        }
    }
    else
    {
        wavwitchbuf = 1;
        if(wavctrl.stop_buf_flag != EM_WAV_TWO_BUFFER_END1)
        {
            wav_buffill(wave_play_buf2, 4096);
        }
        else
        {
            // Stop DMA
            SAI_Play_Stop();
            wavctrl.actived_flag = false;
        }
    }
    wavtransferend = 1;
}
//得到当前播放时间
//fx:文件指针
//wavx:wav播放控制器
void wav_get_curtime(void)
{
    long long fpos;
    wavctrl.totsec = wavctrl.datasize / (wavctrl.bitrate / 8);	//歌曲总长度(单位:秒)
    fpos = wavctrl.current_data - wavctrl.datastart; 					//得到当前文件播放到的地方
    wavctrl.cursec = fpos * wavctrl.totsec / wavctrl.datasize;	//当前播放到第多少秒了?
}
//播放某个WAV文件
//fname:wav文件路径.
//返回值: 0--succeed   1--failse
uint8_t wav_play_song(ENUM_WAVE_TYPES type)
{
    uint8_t res;

    res = wav_decode_init(type); //得到文件的信息
    if(res == 0) //解析文件成功
    {
        if(wavctrl.bps == 16)
        {
            WM8978_I2S_Cfg(2, 0);	//飞利浦标准,16位数据长度
            SAIA_Init(SAI_MODEMASTER_TX, SAI_CLOCKSTROBING_RISINGEDGE, SAI_DATASIZE_16);
            SAIA_SampleRate_Set(wavctrl.samplerate);//设置采样率
            SAIA_TX_DMA_Init(wave_play_buf1, wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE / 2, 1); //配置TX DMA,16位
        }
        else if(wavctrl.bps == 24)
        {
            WM8978_I2S_Cfg(2, 2);	//飞利浦标准,24位数据长度
            SAIA_Init(SAI_MODEMASTER_TX, SAI_CLOCKSTROBING_RISINGEDGE, SAI_DATASIZE_24);
            SAIA_SampleRate_Set(wavctrl.samplerate);//设置采样率
            SAIA_TX_DMA_Init(wave_play_buf1, wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE / 4, 2); //配置TX DMA,32位
        }
        else
        {
            // error format！！！
            return 1;
        }
        sai_tx_callback = wav_sai_dma_tx_callback;			//回调函数指wav_sai_dma_callback

        // 需要先强制停止播放
        SAI_Play_Stop();

        // ===>
        // zzx 初始化缓冲区时 需要同时初始化标记！
        wavctrl.stop_buf_flag = EM_WAV_NO_END;
        // <===
        wavwitchbuf = 0;
        wav_buffill(wave_play_buf1, WAV_SAI_TX_DMA_BUFSIZE);
        wavwitchbuf = 1;
        wav_buffill(wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE);
        SAI_Play_Start();
    }
    return res;
}


void set_wav_control_active_flag(bool active)
{
    wavctrl.actived_flag = active;
}







