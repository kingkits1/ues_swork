#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "wave_data_defines.h"
#include "wavplay.h"
#include "../hardware/sai.h"
#include "../hardware/wm8978.h"
#include "../malloc/mem_defines.h"
#include "../flash/w25qxx.h"
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
    wavctrl.is_ex_flash_data = false;
    buf = (uint8_t *) null;
    switch(wav_type)
    {
    // US
    case EM_WAV_US_CUTTING:   // US MAX 切割
        buf = (uint8_t *)wave_data_us_max;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    case EM_WAV_US_HEMOSTASIS: // US MIN 止血 hemostasis
        buf = (uint8_t *)wave_data_us_min;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    case EM_WAV_US_CLOSING:    // US CLOSING 钳口闭合
        buf = (uint8_t *)wave_data_us_close;
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
        buf = (uint8_t *)wave_data_key_click;
        wavctrl.loop_sound_flag = false;
        wavctrl.actived_flag = true;
        break;
    // POWER ON
    case EM_WAV_POWER_ON:
        wavctrl.is_ex_flash_data = true;
        buf = (uint8_t *)wave_file_head;
        W25QXX1_Read(wave_file_head, FLASH_EX_POWERUP_ADDR, 512);
        wavctrl.flash_start_addr = FLASH_EX_POWERUP_ADDR;

        wavctrl.loop_sound_flag = false;
        wavctrl.actived_flag = true;
        break;
    // SHUT DOWN
    case EM_WAV_SHUT_DOWN:
        wavctrl.is_ex_flash_data = true;
        buf = (uint8_t *)wave_file_head;
        W25QXX1_Read(wave_file_head, FLASH_EX_POWERDOWN_ADDR, 512);
        wavctrl.flash_start_addr = FLASH_EX_POWERDOWN_ADDR;

        wavctrl.loop_sound_flag = false;
        wavctrl.actived_flag = true;
        break;
    // OTHER
    case EM_WAV_DEFAULT:
        break;
    default:
        break;  // TODO:
    }
    // 防止错误输出
    if(buf == (uint8_t *) null) return 1;

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
    wavctrl.bitrate = fmt->ByteRate * 8;		//得到位速
    wavctrl.blockalign = fmt->BlockAlign;		//块对齐
    wavctrl.bps = fmt->BitsPerSample;			//位数,16/24/32位

    wavctrl.datasize = data->ChunkSize;			//数据块大小
    wavctrl.datalocate = wavctrl.datalocate + 8;		//数据流开始的地方.
    //}
    //}
    // zzx 初始化起始数据
    if(wavctrl.is_ex_flash_data == false)
    {
        wavctrl.datastart = buf + wavctrl.datalocate;
        wavctrl.current_data = wavctrl.datastart;
        wavctrl.data_end = wavctrl.datastart + wavctrl.datasize;
        return 0;
    }
    else
    {
        wavctrl.flash_start_addr = wavctrl.flash_start_addr + wavctrl.datalocate;
        wavctrl.flash_cur_addr = wavctrl.flash_start_addr;
        wavctrl.flash_end_addr = wavctrl.flash_start_addr + wavctrl.datasize;
        return 0;
    }
}


//填充buf
//buf:数据区
//size:填充数据量.zzx ！必须为4的倍数
//bits:位数(16/24)
//返回值:读到的数据个数
uint16_t wav_buffill_from_flash(uint8_t *buf, uint16_t size)
{
    uint16_t readlen = 0;
    uint8_t *pbuf;
    uint8_t count;
    uint32_t len;


    // 如果音频数据未准备好，则直接返回！！！
    if(wavctrl.actived_flag == false)
    {
        // Stop DMA
        SAI_Play_Stop();
        return 0;
    }
    // TODO: 这行代码需要验证是否有用！！！
    if(wavctrl.flash_cur_addr >= wavctrl.flash_end_addr) return 0;
    W25QXX1_Read(wav_flash_temp_buf, wavctrl.flash_cur_addr, size);
    wavctrl.current_data = wav_flash_temp_buf;
    pbuf = buf;
    if(wavctrl.bps == 24) //24bit音频,需要处理一下
    {
        len = (uint32_t)size / 4;
        wavctrl.flash_cur_addr += len * 3;
        count = 0;
        while(readlen < size)
        {
            // byte 0
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            // byte 1
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            // byte 2
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            // byte 3 empty with 0
            *pbuf = 0;
            pbuf++;
            readlen ++;
            if(wavctrl.flash_cur_addr >= wavctrl.flash_end_addr)
            {
                // 填充0，并返回播放完成
                while(readlen++ < size)
                {
                    *pbuf = 0;
                    pbuf++;
                }
                if(wavctrl.loop_sound_flag == true)
                {
                    wavctrl.flash_cur_addr = wavctrl.flash_start_addr;
                }
                else
                {
                    //  设置停止标志
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
    else
    {
        wavctrl.flash_cur_addr += size;
        while(readlen < size)
        {
            // byte 0  0-7bit
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            // byte 1 8-15bit
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;

            if(wavctrl.flash_cur_addr >= wavctrl.flash_end_addr)
            {
                // 先填充0，并返回播放完成
                while(readlen++ < size)
                {
                    *pbuf = 0;
                    pbuf++;
                }
                if(wavctrl.loop_sound_flag == true)
                {
                    wavctrl.flash_cur_addr = wavctrl.flash_start_addr;
                }
                else
                {
                    // 设置停止标志
                    if(wavwitchbuf == 0)
                    {
                        wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END0;
                    }
                    else
                    {
                        wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END1;
                    }
                }

                return readlen;
            }
        }
    }
    return readlen;
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
    if(wavctrl.actived_flag == false)
    {
        // Stop DMA
        SAI_Play_Stop();
        return 0;
    }

    pbuf = buf;
    if(wavctrl.bps == 24) //24bit音频,需要处理一下
    {
        count = 0;
        while(readlen < size)
        {
            // byte 0
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            // byte 1
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            // byte 2
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            count++;
            // byte 3 empty with 0
            *pbuf = 0;
            pbuf++;
            readlen ++;
            if(wavctrl.current_data >= wavctrl.data_end)
            {
                // 填充0，并返回播放完成
                while(readlen++ < size)
                {
                    *pbuf = 0;
                    pbuf++;
                }
                if(wavctrl.loop_sound_flag == true)
                {
                    wavctrl.current_data = wavctrl.datastart;
                }
                else
                {
                    // !!!!// 设置停止标志
                    if(wavwitchbuf == 0)
                    {
                        wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END0;
                    }
                    else
                    {
                        wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END1;
                    }
                }
                return readlen;
            }
        }
    }
    else
    {
        while(readlen < size)
        {
            // byte 0  0-7bit
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;
            // byte 1 8-15bit
            *pbuf = *wavctrl.current_data;
            wavctrl.current_data++;
            readlen++;
            pbuf++;

            if(wavctrl.current_data >= wavctrl.data_end)
            {
                // 先填充0，并返回播放完成
                while(readlen++ < size)
                {
                    *pbuf = 0;
                    pbuf++;
                }
                if(wavctrl.loop_sound_flag == true)
                {
                    wavctrl.current_data = wavctrl.datastart;
                }
                else
                {
                    // 设置停止标志
                    if(wavwitchbuf == 0)
                    {
                        wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END0;
                    }
                    else
                    {
                        wavctrl.stop_buf_flag = EM_WAV_TWO_BUFFER_END1;
                    }
                }

                return readlen;
            }
        }
    }
    return readlen;
}
//WAV播放时,SAI DMA传输回调函数
void wav_sai_dma_tx_flash_callback(void)
{
    if(DMA2_Stream3->CR & (1 << 19))
    {
        wavwitchbuf = 0;
        if(wavctrl.stop_buf_flag != EM_WAV_TWO_BUFFER_END1)
        {
            wav_buffill_from_flash(wave_play_buf1, 4096);
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
        if(wavctrl.stop_buf_flag != EM_WAV_TWO_BUFFER_END0)
        {
            wav_buffill_from_flash(wave_play_buf2, 4096);
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
//WAV播放时,SAI DMA传输回调函数
void wav_sai_dma_tx_callback(void)
{
    if(DMA2_Stream3->CR & (1 << 19))
    {
        wavwitchbuf = 0;
        if(wavctrl.stop_buf_flag != EM_WAV_TWO_BUFFER_END1)
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
        if(wavctrl.stop_buf_flag != EM_WAV_TWO_BUFFER_END0)
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
            //SAIA_Init(SAI_MODEMASTER_TX, SAI_CLOCKSTROBING_RISINGEDGE, SAI_DATASIZE_16);
            //SAIA_Init(SAI_MODEMASTER_TX, 0x200, SAI_DATASIZE_16);
            SAIA_SampleRate_Set(wavctrl.samplerate);//设置采样率
            SAIA_TX_DMA_Init(wave_play_buf1, wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE / 2, 1); //配置TX DMA,16位
        }
#if 0
        else if(wavctrl.bps == 24)
        {
            WM8978_I2S_Cfg(2, 2);	//飞利浦标准,24位数据长度
            SAIA_Init(SAI_MODEMASTER_TX, SAI_CLOCKSTROBING_RISINGEDGE, SAI_DATASIZE_24);
            SAIA_SampleRate_Set(wavctrl.samplerate);//设置采样率
            SAIA_TX_DMA_Init(wave_play_buf1, wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE / 4, 2); //配置TX DMA,32位
        }
#endif
        else
        {
            // error format！！！
            return 1;
        }

        if(wavctrl.is_ex_flash_data == false)
        {
            sai_tx_callback = wav_sai_dma_tx_callback;			//回调函数指wav_sai_dma_callback
        }
        else
        {
            sai_tx_callback = wav_sai_dma_tx_flash_callback;
        }
        // 需要先强制停止播放
        SAI_Play_Stop();

        // ===>
        // zzx 初始化缓冲区时 需要同时初始化标记！
        wavctrl.stop_buf_flag = EM_WAV_NO_END;
        // <===
        if(wavctrl.is_ex_flash_data == false)
        {
            wavwitchbuf = 0;
            wav_buffill(wave_play_buf1, WAV_SAI_TX_DMA_BUFSIZE);
            wavwitchbuf = 1;
            wav_buffill(wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE);
        }
        else
        {
            wavwitchbuf = 0;
            wav_buffill_from_flash(wave_play_buf1, WAV_SAI_TX_DMA_BUFSIZE);
            wavwitchbuf = 1;
            wav_buffill_from_flash(wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE);
        }
        wavwitchbuf = 0;
        wavctrl.actived_flag = true;
        SAI_Play_Start();
    }
    return res;
}


void set_wav_control_active_flag(bool active)
{
    wavctrl.actived_flag = active;
}







