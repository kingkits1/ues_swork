#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//V1.0 ˵��
//1,֧��16λ/24λWAV�ļ�����
//2,��߿���֧�ֵ�192K/24bit��WAV��ʽ.
//////////////////////////////////////////////////////////////////////////////////


#define WAV_SAI_TX_DMA_BUFSIZE    4096		//����WAV TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����4096��Ų��Ῠ)

//RIFF��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"RIFF",��0X46464952
    uint32_t ChunkSize ;		   	//���ϴ�С;�ļ��ܴ�С-8
    uint32_t Format;	   			//��ʽ;WAVE,��0X45564157
} ChunkRIFF ;
//fmt��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fmt ",��0X20746D66
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
    uint16_t AudioFormat;	  	//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
    uint16_t NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
    uint32_t SampleRate;			//������;0X1F40,��ʾ8Khz
    uint32_t ByteRate;			//�ֽ�����;
    uint16_t BlockAlign;			//�����(�ֽ�);
    uint16_t BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
    //	uint16_t ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
} ChunkFMT;
//fact��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fact",��0X74636166;
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
    uint32_t NumOfSamples;	  	//����������;
} ChunkFACT;
//LIST��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"LIST",��0X74636166;
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
} ChunkLIST;

//data��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"data",��0X5453494C
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size)
} ChunkDATA;

//wavͷ
typedef __packed struct
{
    ChunkRIFF riff;	//riff��
    ChunkFMT fmt;  	//fmt��
    //	ChunkFACT fact;	//fact�� ����PCM,û������ṹ��
    ChunkDATA data;	//data��
} __WaveHeader;

typedef enum
{
    EM_WAV_NO_END,        // ��ֹͣ����
    EM_WAV_TWO_BUFFER_END0,// ˫����ֹͣ��BUF0
    EM_WAV_TWO_BUFFER_END1,// ˫����ֹͣ��BUF1
    EM_WAV_SINGLE_END
} ENUM_WAV_STOP_BUFFER_DEFINES;
//wav ���ſ��ƽṹ��
typedef __packed struct
{
    uint16_t audioformat;			//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
    uint16_t nchannels;				//ͨ������;1,��ʾ������;2,��ʾ˫����;
    uint16_t blockalign;			//�����(�ֽ�);
    uint32_t datasize;				//WAV���ݴ�С

    uint32_t totsec ;				//���׸�ʱ��,��λ:��
    uint32_t cursec ;				//��ǰ����ʱ��

    uint32_t bitrate;	   			//������(λ��)
    uint32_t samplerate;			//������
    uint16_t bps;					//λ��,����16bit,24bit,32bit

    uint32_t datalocate;			//����֡��ʼ��λ��(���ļ������ƫ��)
    uint8_t *current_data;         // zzx: ��һ�����ֲ������ݵĶ�ȡָ��
    uint8_t *datastart;
    uint8_t *data_end;
    bool loop_sound_flag;           // �ж��Ƿ�Ϊѭ������
    bool actived_flag;              // �Ƿ�Ϊ�״̬
    uint8_t stop_buf_flag;
} __wavctrl;

uint8_t wav_decode_init(ENUM_WAVE_TYPES wav_type);

uint16_t wav_buffill(uint8_t *buf, uint16_t size);
void wav_sai_dma_tx_callback(void);
uint8_t wav_play_song(ENUM_WAVE_TYPES wav_type);
#endif
















