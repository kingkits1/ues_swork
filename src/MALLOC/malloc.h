#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"

#ifndef NULL
#define NULL 0
#endif

//���������ڴ��
#define SRAMIN 0  //�ڲ��ڴ��
#define SRAMEX 1  //�ⲿ�ڴ��(SDRAM)
#define SRAMCCM 2 // CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)

#define SRAMBANK 3 //����֧�ֵ�SRAM����.

// mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE 64 //�ڴ���СΪ64�ֽ�
//#define MEM1_MAX_SIZE			100*1024  						//�������ڴ� 160K  //kang 2021.04.29
#define MEM1_MAX_SIZE 160 * 1024                              //�������ڴ� 160K
#define MEM1_ALLOC_TABLE_SIZE MEM1_MAX_SIZE / MEM1_BLOCK_SIZE //�ڴ���С

// mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSDRAM����
#define MEM2_BLOCK_SIZE 64                                    //�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE 28912 * 1024                            //�������ڴ�28912K
#define MEM2_ALLOC_TABLE_SIZE MEM2_MAX_SIZE / MEM2_BLOCK_SIZE //�ڴ���С

// mem3�ڴ�����趨.mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!)
#define MEM3_BLOCK_SIZE 64                                    //�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE 60 * 1024                               //�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE MEM3_MAX_SIZE / MEM3_BLOCK_SIZE //�ڴ���С

//�ڴ���������
struct _m_mallco_dev
{
    void (*init)(unsigned char);                  //��ʼ��
    unsigned short int (*perused)(unsigned char); //�ڴ�ʹ����
    unsigned char *membase[SRAMBANK];             //�ڴ�� ����SRAMBANK��������ڴ�
    unsigned int *memmap[SRAMBANK];               //�ڴ����״̬��
    unsigned char memrdy[SRAMBANK];               //�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev; //��mallco.c���涨��

void mymemset(void *s, unsigned char c, unsigned int count);        //�����ڴ�
void mymemcpy(void *des, void *src, unsigned int n);                //�����ڴ�
void my_mem_init(unsigned char memx);                               //�ڴ�����ʼ������(��/�ڲ�����)
unsigned int my_mem_malloc(unsigned char memx, unsigned int size);  //�ڴ����(�ڲ�����)
unsigned char my_mem_free(unsigned char memx, unsigned int offset); //�ڴ��ͷ�(�ڲ�����)
unsigned short int my_mem_perused(unsigned char memx);              //����ڴ�ʹ����(��/�ڲ�����)
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(unsigned char memx, void *ptr);                        //�ڴ��ͷ�(�ⲿ����)
void *mymalloc(unsigned char memx, unsigned int size);             //�ڴ����(�ⲿ����)
void *myrealloc(unsigned char memx, void *ptr, unsigned int size); //���·����ڴ�(�ⲿ����)
#endif
