#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "../sys/system.h"
//////////////////////////////////////////////////////////////////////////////////
//V1.0 说明
//1,支持16位/24位WAV文件播放
//2,最高可以支持到192K/24bit的WAV格式.
//////////////////////////////////////////////////////////////////////////////////


#define WAV_SAI_TX_DMA_BUFSIZE    4096		//定义WAV TX DMA 数组大小(播放192Kbps@24bit的时候,需要设置4096大才不会卡)

//RIFF块
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"RIFF",即0X46464952
    uint32_t ChunkSize ;		   	//集合大小;文件总大小-8
    uint32_t Format;	   			//格式;WAVE,即0X45564157
} ChunkRIFF ;
//fmt块
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"fmt ",即0X20746D66
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:20. zzx:0x10
    uint16_t AudioFormat;	  	//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
    uint16_t NumOfChannels;		//通道数量;1,表示单声道;2,表示双声道;
    uint32_t SampleRate;			//采样率;0X1F40,表示8Khz
    uint32_t ByteRate;			//字节速率;
    uint16_t BlockAlign;			//块对齐(字节);
    uint16_t BitsPerSample;		//单个采样数据大小;4位ADPCM,设置为4
    //	uint16_t ByteExtraData;		//附加的数据字节;2个; 线性PCM,没有这个参数
} ChunkFMT;
//fact块
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"fact",即0X74636166;
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4.
    uint32_t NumOfSamples;	  	//采样的数量;
} ChunkFACT;
//LIST块
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"LIST",即0X74636166;zzx:小写"list"
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4.
} ChunkLIST;

//data块
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;这里固定为"data",即0X5453494C zzx:"data"0x64,0x61,0x74,0x61
    uint32_t ChunkSize ;		   	//子集合大小(不包括ID和Size)
} ChunkDATA;

//wav头
typedef __packed struct
{
    ChunkRIFF riff;	//riff块
    ChunkFMT fmt;  	//fmt块
    //	ChunkFACT fact;	//fact块 线性PCM,没有这个结构体
    ChunkDATA data;	//data块
} __WaveHeader;

typedef enum
{
    EM_WAV_NO_END,        // 无停止数据
    EM_WAV_TWO_BUFFER_END0,// 双缓存停止在BUF0
    EM_WAV_TWO_BUFFER_END1,// 双缓存停止在BUF1
    EM_WAV_SINGLE_END
} ENUM_WAV_STOP_BUFFER_DEFINES;
//wav 播放控制结构体
typedef __packed struct
{
    uint16_t audioformat;			//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
    uint16_t nchannels;				//通道数量;1,表示单声道;2,表示双声道;
    uint16_t blockalign;			//块对齐(字节);
    uint32_t datasize;				//WAV数据大小

    uint32_t totsec ;				//整首歌时长,单位:秒
    uint32_t cursec ;				//当前播放时长

    uint32_t bitrate;	   			//比特率(位速)
    uint32_t samplerate;			//采样率
    uint16_t bps;					//位数,比如16bit,24bit,32bit

    uint32_t datalocate;			//数据帧开始的位置(在文件里面的偏移)
    uint8_t *current_data;         // zzx: 下一包音乐播放数据的读取指针
    uint8_t *datastart;
    uint8_t *data_end;
    bool loop_sound_flag;           // 判定是否为循环播放
    bool actived_flag;              // 是否为活动状态
    uint8_t stop_buf_flag;

    bool is_ex_flash_data;
    uint32_t flash_start_addr;
    uint32_t flash_end_addr;
    uint32_t flash_cur_addr;
} __wavctrl;

uint8_t wav_decode_init(ENUM_WAVE_TYPES wav_type);
uint16_t wav_buffill(uint8_t *buf, uint16_t size);
uint16_t wav_buffill_from_flash(uint8_t *buf, uint16_t size);
void wav_sai_dma_tx_callback(void);
void wav_sai_dma_tx_flash_callback(void);
uint8_t wav_play_song(ENUM_WAVE_TYPES wav_type);
void set_wav_control_active_flag(bool active);
#endif
















