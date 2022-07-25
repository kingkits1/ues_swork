#include "wavplay.h" 
#include "audioplay.h"
#include "sai.h"
#include "wm8978.h"
#include "../malloc/mem_defines.h"
#include "wave_data_defines.h"
//////////////////////////////////////////////////////////////////////////////////	 
//1,支持16位/24位WAV文件播放
//2,最高可以支持到192K/24bit的WAV格式. 
////////////////////////////////////////////////////////////////////////////////// 	

////////
__wavctrl wavctrl;		//WAV控制结构体
volatile uint8_t wavtransferend=0;	//sai传输完成标志
volatile uint8_t wavwitchbuf=0;		//saibufx指示标志


//WAV解析初始化
//wavx:wav 信息存放结构体指针
//返回值:0,成功;1,打开文件失败;2,非WAV文件;3,DATA区域未找到.
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
        default:break;  // TODO:
    }
    // 防止错误输出
    if(buf==wave_file_head) return 1;
    
	riff=(ChunkRIFF *)buf;		//获取RIFF块
	//if(riff->Format==0X45564157)//是WAV文件
	//{
	fmt=(ChunkFMT *)(buf+12);	//获取FMT块 
	fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//读取FACT块
	if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)
	{
	    wavctrl.datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//具有fact/LIST块的时候(未测试)
	}
	else
	{
	    wavctrl.datastart=12+8+fmt->ChunkSize;  
	}

	// zzx 初始化起始数据
	wavctrl.current_data_ptr = wavctrl.datastart;
	data=(ChunkDATA *)(buf+wavctrl.datastart);	//读取DATA块
	//if(data->ChunkID==0X61746164)//解析成功!
	//{
	wavctrl.audioformat=fmt->AudioFormat;		//音频格式
	wavctrl.nchannels=fmt->NumOfChannels;		//通道数
	wavctrl.samplerate=fmt->SampleRate;		//采样率
	wavctrl.bitrate=fmt->ByteRate*8;			//得到位速
	wavctrl.blockalign=fmt->BlockAlign;		//块对齐
	wavctrl.bps=fmt->BitsPerSample;			//位数,16/24/32位
	
	wavctrl.datasize=data->ChunkSize;			//数据块大小
	wavctrl.datastart=wavctrl.datastart+8;		//数据流开始的地方. 
	//}
	//}
	
	return 0;
}

//填充buf
//buf:数据区
//size:填充数据量
//bits:位数(16/24)
//返回值:读到的数据个数
uint32_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits)
{
	uint16_t readlen=0;
	uint32_t bread;
	uint16_t i;
	uint32_t *p,*pbuf;
	if(bits==24)//24bit音频,需要处理一下
	{
		readlen=(size/4)*3;		//此次要读取的字节数
		f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);//读取数据 
		pbuf=(uint32_t*)buf;
		for(i=0;i<size/4;i++)
		{  
			p=(uint32_t*)(audiodev.tbuf+i*3);
			pbuf[i]=p[0];  
		} 
		bread=(bread*4)/3;		//填充后的大小.
	}else 
	{
		f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit音频,直接读取数据  
		if(bread<size)//不够数据了,补充0
		{
			for(i=bread;i<size-bread;i++)buf[i]=0; 
		}
	}
	return bread;
}  
//WAV播放时,SAI DMA传输回调函数
void wav_sai_dma_tx_callback(void) 
{   
	uint16_t i;
	if(DMA2_Stream3->CR&(1<<19))
	{
		wavwitchbuf=0;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_SAI_TX_DMA_BUFSIZE;i++)//暂停
			{
				audiodev.saibuf1[i]=0;//填充0
			}
		}
	}else 
	{
		wavwitchbuf=1;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_SAI_TX_DMA_BUFSIZE;i++)//暂停
			{
				audiodev.saibuf2[i]=0;//填充0
			}
		}
	}
	wavtransferend=1;
} 
//得到当前播放时间
//fx:文件指针
//wavx:wav播放控制器
void wav_get_curtime(FIL*fx,__wavctrl *wavx)
{
	long long fpos;  	
 	wavctrl.totsec=wavctrl.datasize/(wavctrl.bitrate/8);	//歌曲总长度(单位:秒) 
	fpos=fx->fptr-wavctrl.datastart; 					//得到当前文件播放到的地方 
	wavctrl.cursec=fpos*wavctrl.totsec/wavctrl.datasize;	//当前播放到第多少秒了?	
}
//播放某个WAV文件
//fname:wav文件路径.
//返回值:
//KEY0_PRES:下一曲
//KEY1_PRES:上一曲
//其他:错误
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
		res=wav_decode_init(fname,&wavctrl);//得到文件的信息
		if(res==0)//解析文件成功
		{
			if(wavctrl.bps==16)
			{
				WM8978_I2S_Cfg(2,0);	//飞利浦标准,16位数据长度
                SAIA_Init(SAI_MODEMASTER_TX,SAI_CLOCKSTROBING_RISINGEDGE,SAI_DATASIZE_16);
				SAIA_SampleRate_Set(wavctrl.samplerate);//设置采样率  
                SAIA_TX_DMA_Init(audiodev.saibuf1,audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE/2,1); //配置TX DMA,16位
			}else if(wavctrl.bps==24)
			{
				WM8978_I2S_Cfg(2,2);	//飞利浦标准,24位数据长度
                SAIA_Init(SAI_MODEMASTER_TX,SAI_CLOCKSTROBING_RISINGEDGE,SAI_DATASIZE_24);
				SAIA_SampleRate_Set(wavctrl.samplerate);//设置采样率
                SAIA_TX_DMA_Init(audiodev.saibuf1,audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE/4,2); //配置TX DMA,32位                
			}
			sai_tx_callback=wav_sai_dma_tx_callback;			//回调函数指wav_sai_dma_callback 
			audio_stop();
			res=f_open(audiodev.file,(TCHAR*)fname,FA_READ);	//打开文件
			if(res==0)
			{
				f_lseek(audiodev.file, wavctrl.datastart);		//跳过文件头
				fillnum=wav_buffill(audiodev.saibuf1,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);
				fillnum=wav_buffill(audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);
				audio_start();  
				while(res==0)
				{ 
					while(wavtransferend==0);//等待wav传输完成; 
					wavtransferend=0;
					if(fillnum!=WAV_SAI_TX_DMA_BUFSIZE)//播放结束?
					{
						res=KEY0_PRES;
						break;
					} 
 					if(wavwitchbuf)fillnum=wav_buffill(audiodev.saibuf2,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);//填充buf2
					else fillnum=wav_buffill(audiodev.saibuf1,WAV_SAI_TX_DMA_BUFSIZE,wavctrl.bps);//填充buf1
					while(1)
					{
						key=KEY_Scan(0); 
						if(key==WKUP_PRES)//暂停
						{
							if(audiodev.status&0X01)audiodev.status&=~(1<<0);
							else audiodev.status|=0X01;  
						}
						if(key==KEY2_PRES||key==KEY0_PRES)//下一曲/上一曲
						{
							res=key;
							break; 
						}
						wav_get_curtime(audiodev.file,&wavctrl);//得到总时间和当前播放的时间 
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
	myfree(SRAMIN,audiodev.tbuf);	//释放内存
	myfree(SRAMIN,audiodev.saibuf1);//释放内存
	myfree(SRAMIN,audiodev.saibuf2);//释放内存 
	myfree(SRAMIN,audiodev.file);	//释放内存 
	return res;
} 
	









