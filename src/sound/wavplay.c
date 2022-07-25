#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "wave_data_defines.h"
#include "wavplay.h"
#include "../hardware/sai.h"
#include "../hardware/wm8978.h"
#include "../malloc/mem_defines.h"
//////////////////////////////////////////////////////////////////////////////////
//1,֧��16λ/24λWAV�ļ�����
//2,��߿���֧�ֵ�192K/24bit��WAV��ʽ.
//////////////////////////////////////////////////////////////////////////////////

////////
__wavctrl wavctrl;		//WAV���ƽṹ��
volatile uint8_t wavtransferend = 0;	//sai������ɱ�־
volatile uint8_t wavwitchbuf = 0;		//saibufxָʾ��־


//WAV������ʼ��
//wavx:wav ��Ϣ��Žṹ��ָ��
//����ֵ:0,�ɹ�;1,���ļ�ʧ��;2,��WAV�ļ�;3,DATA����δ�ҵ�.
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
    case EM_WAV_US_CUTTING:   // US MAX �и�
        buf = wave_data_us_max;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    case EM_WAV_US_HEMOSTASIS: // US MIN ֹѪ hemostasis
        buf = wave_data_us_min;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    case EM_WAV_US_CLOSING:    // US CLOSING ǯ�ڱպ�
        buf = wave_data_us_close;
        wavctrl.loop_sound_flag = true;
        wavctrl.actived_flag = true;
        break;
    // ALARM
    case EM_WAV_ALARM_LOW:     // �����ȼ���ʾ��
        break;
    case EM_WAV_ALARM_HIGH:    // �����ȼ���ʾ��
        break;
    // ES
    case EM_WAV_ES_CUTTING:    // ES MAX �и�
        break;
    case EM_WAV_ES_HEMOSTASIS: // ES MIN ֹѪ hemostasis
        break;
    case EM_WAV_ES_CLOSING:    // ES CLOSING �պ�
        break;
    // US+ES
    case EM_WAV_UES_CUTTING:   // US+ES MAX �и�
        break;
    case EM_WAV_UES_HEMOSTASIS: // US+ES MIN ֹѪ hemostasis
        break;
    case EM_WAV_UES_CLOSING:    // US+ES CLOSING �պ�
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
    // ��ֹ�������
    if(buf == wave_file_head) return 1;

    //riff=(ChunkRIFF *)buf;		//��ȡRIFF��
    //if(riff->Format==0X45564157)//��WAV�ļ�
    //{
    fmt = (ChunkFMT *)(buf + 12);	//��ȡFMT��
    fact = (ChunkFACT *)(buf + 12 + 8 + fmt->ChunkSize); //��ȡFACT��
    if(fact->ChunkID == 0X74636166 || fact->ChunkID == 0X5453494C)
    {
        wavctrl.datalocate = 12 + 8 + fmt->ChunkSize + 8 + fact->ChunkSize; //����fact/LIST���ʱ��(δ����)
    }
    else
    {
        wavctrl.datalocate = 12 + 8 + fmt->ChunkSize;
    }


    data = (ChunkDATA *)(buf + wavctrl.datalocate);	//��ȡDATA��
    //if(data->ChunkID==0X61746164)//�����ɹ�!
    //{
    wavctrl.audioformat = fmt->AudioFormat;		//��Ƶ��ʽ
    wavctrl.nchannels = fmt->NumOfChannels;		//ͨ����
    wavctrl.samplerate = fmt->SampleRate;		//������
    wavctrl.bitrate = fmt->ByteRate * 8;			//�õ�λ��
    wavctrl.blockalign = fmt->BlockAlign;		//�����
    wavctrl.bps = fmt->BitsPerSample;			//λ��,16/24/32λ

    wavctrl.datasize = data->ChunkSize;			//���ݿ��С
    wavctrl.datalocate = wavctrl.datalocate + 8;		//��������ʼ�ĵط�.
    //}
    //}
    // zzx ��ʼ����ʼ����
    wavctrl.datastart = buf + wavctrl.datalocate;
    wavctrl.current_data = wavctrl.datastart;
    wavctrl.data_end = wavctrl.datastart + wavctrl.datasize;
    return 0;
}

//���buf
//buf:������
//size:���������.zzx ������Ϊ4�ı���
//bits:λ��(16/24)
//����ֵ:���������ݸ���
uint16_t wav_buffill(uint8_t *buf, uint16_t size)
{
    uint16_t readlen = 0;
    uint8_t *pbuf;
    uint8_t count;
    // �����Ƶ����δ׼���ã���ֱ�ӷ��أ�����
    if(wavctrl.actived_flag == false)	return 0;

    pbuf = buf;
    if(wavctrl.bps == 24) //24bit��Ƶ,��Ҫ����һ��
    {
        // zzx:24bitʱ��ʵ�ʴ洢��ÿ3���ֽ�Ϊһ�����ݣ�
        // ��������ʱ����Ҫд��4�ֽڣ�����ֽڵ�����
        // ����Ч�ģ���������3�ֽ�ת4�ֽڣ�
        // ������Ҫ�������ݲ���һ��������ʱ�����⣡����
        // ��ʱҪ�����Ƿ�Ϊѭ�����ݣ�������

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
                // ��������
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
                    // ���0�������ز������
                    while(readlen++ < size)
                    {
                        *pbuf = 0;
                        pbuf++;
                        // TODO: !!!!// ����ֹͣ��־
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
                    // ���0�������ز������
                    while(readlen++ < size)
                    {
                        *pbuf = 0;
                        pbuf++;
                        //TODO: !!!!// ����ֹͣ��־
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
//WAV����ʱ,SAI DMA����ص�����
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
//�õ���ǰ����ʱ��
//fx:�ļ�ָ��
//wavx:wav���ſ�����
void wav_get_curtime(void)
{
    long long fpos;
    wavctrl.totsec = wavctrl.datasize / (wavctrl.bitrate / 8);	//�����ܳ���(��λ:��)
    fpos = wavctrl.current_data - wavctrl.datastart; 					//�õ���ǰ�ļ����ŵ��ĵط�
    wavctrl.cursec = fpos * wavctrl.totsec / wavctrl.datasize;	//��ǰ���ŵ��ڶ�������?
}
//����ĳ��WAV�ļ�
//fname:wav�ļ�·��.
//����ֵ: 0--succeed   1--failse
uint8_t wav_play_song(ENUM_WAVE_TYPES type)
{
    uint8_t res;

    res = wav_decode_init(type); //�õ��ļ�����Ϣ
    if(res == 0) //�����ļ��ɹ�
    {
        if(wavctrl.bps == 16)
        {
            WM8978_I2S_Cfg(2, 0);	//�����ֱ�׼,16λ���ݳ���
            SAIA_Init(SAI_MODEMASTER_TX, SAI_CLOCKSTROBING_RISINGEDGE, SAI_DATASIZE_16);
            SAIA_SampleRate_Set(wavctrl.samplerate);//���ò�����
            SAIA_TX_DMA_Init(wave_play_buf1, wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE / 2, 1); //����TX DMA,16λ
        }
        else if(wavctrl.bps == 24)
        {
            WM8978_I2S_Cfg(2, 2);	//�����ֱ�׼,24λ���ݳ���
            SAIA_Init(SAI_MODEMASTER_TX, SAI_CLOCKSTROBING_RISINGEDGE, SAI_DATASIZE_24);
            SAIA_SampleRate_Set(wavctrl.samplerate);//���ò�����
            SAIA_TX_DMA_Init(wave_play_buf1, wave_play_buf2, WAV_SAI_TX_DMA_BUFSIZE / 4, 2); //����TX DMA,32λ
        }
        else
        {
            // error format������
            return 1;
        }
        sai_tx_callback = wav_sai_dma_tx_callback;			//�ص�����ָwav_sai_dma_callback

        // ��Ҫ��ǿ��ֹͣ����
        SAI_Play_Stop();

        // ===>
        // zzx ��ʼ��������ʱ ��Ҫͬʱ��ʼ����ǣ�
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







