#include "wavplay.h" 
#include "audioplay.h"
#include "sai.h"
#include "wm8978.h"
#include "../malloc/mem_defines.h"
#include "wave_data_defines.h"
//////////////////////////////////////////////////////////////////////////////////	 
//1,֧��16λ/24λWAV�ļ�����
//2,��߿���֧�ֵ�192K/24bit��WAV��ʽ. 
////////////////////////////////////////////////////////////////////////////////// 	

////////
__wavctrl wavctrl;		//WAV���ƽṹ��
volatile uint8_t wavtransferend=0;	//sai������ɱ�־
volatile uint8_t wavwitchbuf=0;		//saibufxָʾ��־


//WAV������ʼ��
//wavx:wav ��Ϣ��Žṹ��ָ��
//����ֵ:0,�ɹ�;1,���ļ�ʧ��;2,��WAV�ļ�;3,DATA����δ�ҵ�.
uint8_t wav_decode_init(ENUM_WAVE_TYPES wav_type)
{
	uint8_t *buf; 	
	ChunkRIFF *riff;
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
        default:break;  // TODO:
    }
    // ��ֹ�������
    if(buf==wave_file_head) return 1;
    
	riff=(ChunkRIFF *)buf;		//��ȡRIFF��
	//if(riff->Format==0X45564157)//��WAV�ļ�
	//{
	fmt=(ChunkFMT *)(buf+12);	//��ȡFMT�� 
	fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//��ȡFACT��
	if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)
	{
	    wavctrl.datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//����fact/LIST���ʱ��(δ����)
	}
	else
	{
	    wavctrl.datastart=12+8+fmt->ChunkSize;  
	}

	// zzx ��ʼ����ʼ����
	wavctrl.current_data_ptr = wavctrl.datastart;
	data=(ChunkDATA *)(buf+wavctrl.datastart);	//��ȡDATA��
	//if(data->ChunkID==0X61746164)//�����ɹ�!
	//{
	wavctrl.audioformat=fmt->AudioFormat;		//��Ƶ��ʽ
	wavctrl.nchannels=fmt->NumOfChannels;		//ͨ����
	wavctrl.samplerate=fmt->SampleRate;		//������
	wavctrl.bitrate=fmt->ByteRate*8;			//�õ�λ��
	wavctrl.blockalign=fmt->BlockAlign;		//�����
	wavctrl.bps=fmt->BitsPerSample;			//λ��,16/24/32λ
	
	wavctrl.datasize=data->ChunkSize;			//���ݿ��С
	wavctrl.datastart=wavctrl.datastart+8;		//��������ʼ�ĵط�. 
	//}
	//}
	
	return 0;
}

//���buf
//buf:������
//size:���������
//bits:λ��(16/24)
//����ֵ:���������ݸ���
uint32_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits)
{
	uint16_t readlen=0;
	uint32_t bread;
	uint16_t i;
	uint32_t *p,*pbuf;
	if(bits==24)//24bit��Ƶ,��Ҫ����һ��
	{
		readlen=(size/4)*3;		//�˴�Ҫ��ȡ���ֽ���
		f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);//��ȡ���� 
		pbuf=(uint32_t*)buf;
		for(i=0;i<size/4;i++)
		{  
			p=(uint32_t*)(audiodev.tbuf+i*3);
			pbuf[i]=p[0];  
		} 
		bread=(bread*4)/3;		//����Ĵ�С.
	}else 
	{
		f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit��Ƶ,ֱ�Ӷ�ȡ����  
		if(bread<size)//����������,����0
		{
			for(i=bread;i<size-bread;i++)buf[i]=0; 
		}
	}
	return bread;
}  
//WAV����ʱ,SAI DMA����ص�����
void wav_sai_dma_tx_callback(void) 
{   
	uint16_t i;
	if(DMA2_Stream3->CR&(1<<19))
	{
		wavwitchbuf=0;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_SAI_TX_DMA_BUFSIZE;i++)//��ͣ
			{
				audiodev.saibuf1[i]=0;//���0
			}
		}
	}else 
	{
		wavwitchbuf=1;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_SAI_TX_DMA_BUFSIZE;i++)//��ͣ
			{
				audiodev.saibuf2[i]=0;//���0
			}
		}
	}
	wavtransferend=1;
} 
//�õ���ǰ����ʱ��
//fx:�ļ�ָ��
//wavx:wav���ſ�����
void wav_get_curtime(FIL*fx,__wavctrl *wavx)
{
	long long fpos;  	
 	wavctrl.totsec=wavctrl.datasize/(wavctrl.bitrate/8);	//�����ܳ���(��λ:��) 
	fpos=fx->fptr-wavctrl.datastart; 					//�õ���ǰ�ļ����ŵ��ĵط� 
	wavctrl.cursec=fpos*wavctrl.totsec/wavctrl.datasize;	//��ǰ���ŵ��ڶ�������?	
}
//����ĳ��WAV�ļ�
//fname:wav�ļ�·��.
//����ֵ:
//KEY0_PRES:��һ��
//KEY1_PRES:��һ��
//����:����
uint8_t wav_play_song(uint8_t* fname)
{
	uint8_t key;
	uint8_t t=0; 
	uint8_t res;  
	uint32_t fillnum; 
	audiodev.file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	audiodev.saibuf1=mymalloc(SRAMIN,WAV_SAI_TX_DMA_BUFSIZE);
	audiodev.saibuf2=mymalloc(SRAMIN,WAV_SAI_TX_DMA_BUFSIZE);
	audiodev.tbuf=mymalloc(SRAMIN,WAV_SAI_TX_DMA_BUFSIZE);
	if(audiodev.file&&audiodev.saibuf1&&audiodev.saibuf2&&audiodev.tbuf)
	{ 
		res=wav_decode_init(fname,&wavctrl);//�õ��ļ�����Ϣ
		if(res==0)//�����ļ��ɹ�
		{
			if(wavctrl.bps==16)
			{
				WM8978_I2S_Cfg(2,0);	//�����ֱ�׼,16λ���ݳ���
                SAIA_Init(SAI_MODEMASTER_TX,SAI_CLOCKSTROBING_RISINGEDGE,SAI_DATASIZE_16);
				SAIA_SampleRate_Set(wavctrl.samplerate);//���ò�����  
                SAIA_TX_DMA_Init(audiodev.saibuf1,audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE/2,1); //����TX DMA,16λ
			}else if(wavctrl.bps==24)
			{
				WM8978_I2S_Cfg(2,2);	//�����ֱ�׼,24λ���ݳ���
                SAIA_Init(SAI_MODEMASTER_TX,SAI_CLOCKSTROBING_RISINGEDGE,SAI_DATASIZE_24);
				SAIA_SampleRate_Set(wavctrl.samplerate);//���ò�����
                SAIA_TX_DMA_Init(audiodev.saibuf1,audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE/4,2); //����TX DMA,32λ                
			}
			sai_tx_callback=wav_sai_dma_tx_callback;			//�ص�����ָwav_sai_dma_callback 
			audio_stop();
			res=f_open(audiodev.file,(TCHAR*)fname,FA_READ);	//���ļ�
			if(res==0)
			{
				f_lseek(audiodev.file, wavctrl.datastart);		//�����ļ�ͷ
				fillnum=wav_buffill(audiodev.saibuf1,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);
				fillnum=wav_buffill(audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);
				audio_start();  
				while(res==0)
				{ 
					while(wavtransferend==0);//�ȴ�wav�������; 
					wavtransferend=0;
					if(fillnum!=WAV_SAI_TX_DMA_BUFSIZE)//���Ž���?
					{
						res=KEY0_PRES;
						break;
					} 
 					if(wavwitchbuf)fillnum=wav_buffill(audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);//���buf2
					else fillnum=wav_buffill(audiodev.saibuf1,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);//���buf1
					while(1)
					{
						key=KEY_Scan(0); 
						if(key==WKUP_PRES)//��ͣ
						{
							if(audiodev.status&0X01)audiodev.status&=~(1<<0);
							else audiodev.status|=0X01;  
						}
						if(key==KEY2_PRES||key==KEY0_PRES)//��һ��/��һ��
						{
							res=key;
							break; 
						}
						wav_get_curtime(audiodev.file,&wavctrl);//�õ���ʱ��͵�ǰ���ŵ�ʱ�� 
						audio_msg_show(wavctrl.totsec,wavctrl.cursec,wavctrl.bitrate);
						t++;
						if(t==20)
						{
							t=0;
 							LED0=!LED0;
						}
						if((audiodev.status&0X01)==0)delay_ms(10);
						else break;
					}
				}
				audio_stop(); 
			}else res=0XFF; 
		}else res=0XFF;
	}else res=0XFF; 
	myfree(SRAMIN,audiodev.tbuf);	//�ͷ��ڴ�
	myfree(SRAMIN,audiodev.saibuf1);//�ͷ��ڴ�
	myfree(SRAMIN,audiodev.saibuf2);//�ͷ��ڴ� 
	myfree(SRAMIN,audiodev.file);	//�ͷ��ڴ� 
	return res;
} 
	









