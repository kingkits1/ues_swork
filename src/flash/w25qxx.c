#include "w25qxx.h"
#include "stm32f4xx_hal_gpio.h"

/* ****************************************************
    һ����2Ƭw25q256
 */
extern SPI_HandleTypeDef hspi5;
static uint8_t SPI_RX_TMP_BYTE;
uint16_t W25QXX_TYPE = W25Q256;	//Ĭ����W25Q256

static void send_byte_to_flash(uint8_t ch)
{
    HAL_SPI_Transmit(&hspi5, &ch, 1, 1000);
}

static uint8_t read_byte_from_flash(uint8_t ch)
{
    HAL_SPI_TransmitReceive(&hspi5, &ch, &SPI_RX_TMP_BYTE, 1, 1000);
    return SPI_RX_TMP_BYTE;
}
//��ȡоƬID
//����ֵ����:
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128
//0XEF18,��ʾоƬ�ͺ�ΪW25Q256
uint16_t W25QXX1_ReadID(void)
{
    uint16_t Temp = 0;
    W25QXX_CS1 = 0;
    send_byte_to_flash(0x90);//���Ͷ�ȡID����
    send_byte_to_flash(0x00);
    send_byte_to_flash(0x00);
    send_byte_to_flash(0x00);
    Temp |= read_byte_from_flash(0xFF) << 8;
    Temp |= read_byte_from_flash(0xFF);
    W25QXX_CS1 = 1;
    return Temp;
}
uint16_t W25QXX2_ReadID(void)
{
    uint16_t Temp = 0;
    W25QXX_CS2 = 0;
    send_byte_to_flash(0x90);//���Ͷ�ȡID����
    send_byte_to_flash(0x00);
    send_byte_to_flash(0x00);
    send_byte_to_flash(0x00);
    Temp |= read_byte_from_flash(0xFF) << 8;
    Temp |= read_byte_from_flash(0xFF);
    W25QXX_CS2 = 1;
    return Temp;
}
//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
uint8_t W25QXX1_ReadSR(uint8_t regno)
{
    uint8_t byte = 0, command = 0;
    switch(regno)
    {
    case 1:
        command = W25X_ReadStatusReg1;  //��״̬�Ĵ���1ָ��
        break;
    case 2:
        command = W25X_ReadStatusReg2;  //��״̬�Ĵ���2ָ��
        break;
    case 3:
        command = W25X_ReadStatusReg3;  //��״̬�Ĵ���3ָ��
        break;
    default:
        command = W25X_ReadStatusReg1;
        break;
    }
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(command);            //���Ͷ�ȡ״̬�Ĵ�������
    byte = read_byte_from_flash(0Xff);        //��ȡһ���ֽ�
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
    return byte;
}
uint8_t W25QXX2_ReadSR(uint8_t regno)
{
    uint8_t byte = 0, command = 0;
    switch(regno)
    {
    case 1:
        command = W25X_ReadStatusReg1;  //��״̬�Ĵ���1ָ��
        break;
    case 2:
        command = W25X_ReadStatusReg2;  //��״̬�Ĵ���2ָ��
        break;
    case 3:
        command = W25X_ReadStatusReg3;  //��״̬�Ĵ���3ָ��
        break;
    default:
        command = W25X_ReadStatusReg1;
        break;
    }
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(command);            //���Ͷ�ȡ״̬�Ĵ�������
    byte = read_byte_from_flash(0Xff);        //��ȡһ���ֽ�
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
    return byte;
}
//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q256
//����Ϊ32M�ֽ�,����512��Block,8192��Sector

//��ʼ��SPI FLASH��IO��
void W25QXX1_Init(void)
{
    uint8_t temp;
    W25QXX_TYPE = W25QXX1_ReadID();	        	//��ȡFLASH ID.
    if(W25QXX_TYPE == W25Q256)              //SPI FLASHΪW25Q256
    {
        temp = W25QXX1_ReadSR(3);            //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp & 0X01) == 0)			        		//�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
        {
            W25QXX_CS1 = 0; 			       								 //ѡ��
            send_byte_to_flash(W25X_Enable4ByteAddr);
            W25QXX_CS1 = 1;       		       						 //ȡ��Ƭѡ
        }
    }
    W25QXX_CS1 = 1;
}
void W25QXX2_Init(void)
{
    uint8_t temp;
    W25QXX_TYPE = W25QXX2_ReadID();	        	//��ȡFLASH ID.
    if(W25QXX_TYPE == W25Q256)              //SPI FLASHΪW25Q256
    {
        temp = W25QXX2_ReadSR(3);            //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp & 0X01) == 0)			        		//�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
        {
            W25QXX_CS2 = 0; 			       								 //ѡ��
            send_byte_to_flash(W25X_Enable4ByteAddr);
            W25QXX_CS2 = 1;       		       						 //ȡ��Ƭѡ
        }
    }
    W25QXX_CS2 = 1; 
}



//дW25QXX״̬�Ĵ���
void W25QXX1_Write_SR(uint8_t regno, uint8_t sr)
{
    uint8_t command = 0;
    switch(regno)
    {
    case 1:
        command = W25X_WriteStatusReg1;  //д״̬�Ĵ���1ָ��
        break;
    case 2:
        command = W25X_WriteStatusReg2;  //д״̬�Ĵ���2ָ��
        break;
    case 3:
        command = W25X_WriteStatusReg3;  //д״̬�Ĵ���3ָ��
        break;
    default:
        command = W25X_WriteStatusReg1;
        break;
    }
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(command);            //����дȡ״̬�Ĵ�������
    send_byte_to_flash(sr);                 //д��һ���ֽ�
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
}

void W25QXX2_Write_SR(uint8_t regno, uint8_t sr)
{
    uint8_t command = 0;
    switch(regno)
    {
    case 1:
        command = W25X_WriteStatusReg1;  //д״̬�Ĵ���1ָ��
        break;
    case 2:
        command = W25X_WriteStatusReg2;  //д״̬�Ĵ���2ָ��
        break;
    case 3:
        command = W25X_WriteStatusReg3;  //д״̬�Ĵ���3ָ��
        break;
    default:
        command = W25X_WriteStatusReg1;
        break;
    }
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(command);            //����дȡ״̬�Ĵ�������
    send_byte_to_flash(sr);                 //д��һ���ֽ�
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
}
//W25QXXдʹ��
//��WEL��λ
void W25QXX1_Write_Enable(void)
{
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_WriteEnable);   //����дʹ��
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
}
void W25QXX2_Write_Enable(void)
{
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_WriteEnable);   //����дʹ��
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
}
//W25QXXд��ֹ
//��WEL����
void W25QXX1_Write_Disable(void)
{
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_WriteDisable);  //����д��ָֹ��
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
}
void W25QXX2_Write_Disable(void)
{
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_WriteDisable);  //����д��ָֹ��
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX1_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_ReadData);      //���Ͷ�ȡ����
    if(W25QXX_TYPE == W25Q256)              //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        send_byte_to_flash((uint8_t)((ReadAddr) >> 24));
    }
    send_byte_to_flash((uint8_t)((ReadAddr) >> 16)); //����24bit��ַ
    send_byte_to_flash((uint8_t)((ReadAddr) >> 8));
    send_byte_to_flash((uint8_t)ReadAddr);

    HAL_SPI_Receive(&hspi5, pBuffer, NumByteToRead, 1000);
    W25QXX_CS1 = 1;
}
void W25QXX2_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_ReadData);      //���Ͷ�ȡ����
    if(W25QXX_TYPE == W25Q256)              //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        send_byte_to_flash((uint8_t)((ReadAddr) >> 24));
    }
    send_byte_to_flash((uint8_t)((ReadAddr) >> 16)); //����24bit��ַ
    send_byte_to_flash((uint8_t)((ReadAddr) >> 8));
    send_byte_to_flash((uint8_t)ReadAddr);

    HAL_SPI_Receive(&hspi5, pBuffer, NumByteToRead, 1000);
    W25QXX_CS2 = 1;
}

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void W25QXX1_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    W25QXX1_Write_Enable();                  //SET WEL
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_PageProgram);   //����дҳ����
    if(W25QXX_TYPE == W25Q256)              //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        send_byte_to_flash((uint8_t)((WriteAddr) >> 24));
    }
    send_byte_to_flash((uint8_t)((WriteAddr) >> 16)); //����24bit��ַ
    send_byte_to_flash((uint8_t)((WriteAddr) >> 8));
    send_byte_to_flash((uint8_t)WriteAddr);
    HAL_SPI_Transmit(&hspi5, pBuffer, NumByteToWrite, 1000);
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
    W25QXX1_Wait_Busy();					   //�ȴ�д�����
}

void W25QXX2_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    W25QXX2_Write_Enable();                  //SET WEL
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_PageProgram);   //����дҳ����
    if(W25QXX_TYPE == W25Q256)              //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        send_byte_to_flash((uint8_t)((WriteAddr) >> 24));
    }
    send_byte_to_flash((uint8_t)((WriteAddr) >> 16)); //����24bit��ַ
    send_byte_to_flash((uint8_t)((WriteAddr) >> 8));
    send_byte_to_flash((uint8_t)WriteAddr);
    HAL_SPI_Transmit(&hspi5, pBuffer, NumByteToWrite, 1000);
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
    W25QXX2_Wait_Busy();					   //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX1_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
    if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //������256���ֽ�
    while(1)
    {
        W25QXX1_Write_Page(pBuffer, WriteAddr, pageremain);
        if(NumByteToWrite == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			 //��ȥ�Ѿ�д���˵��ֽ���
            if(NumByteToWrite > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = NumByteToWrite; 	 //����256���ֽ���
        }
    };
}
void W25QXX2_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
    if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //������256���ֽ�
    while(1)
    {
        W25QXX2_Write_Page(pBuffer, WriteAddr, pageremain);
        if(NumByteToWrite == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			 //��ȥ�Ѿ�д���˵��ֽ���
            if(NumByteToWrite > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = NumByteToWrite; 	 //����256���ֽ���
        }
    };
}
//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
uint8_t W25QXX_BUFFER[4096];
void W25QXX1_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //������ַ
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С
    if(NumByteToWrite <= secremain)secremain = NumByteToWrite; //������4096���ֽ�
    while(1)
    {
        W25QXX1_Read(W25QXX_BUF, secpos * 4096, 4096); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(W25QXX_BUF[secoff + i] != 0XFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            W25QXX1_Erase_Sector(secpos);//�����������
            for(i = 0; i < secremain; i++)	 //����
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX1_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //д����������

        }
        else W25QXX1_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        if(NumByteToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��
            NumByteToWrite -= secremain;				//�ֽ����ݼ�
            if(NumByteToWrite > 4096)secremain = 4096;	//��һ����������д����
            else secremain = NumByteToWrite;			//��һ����������д����
        }
    };
}

void W25QXX2_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //������ַ
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С
    if(NumByteToWrite <= secremain)secremain = NumByteToWrite; //������4096���ֽ�
    while(1)
    {
        W25QXX2_Read(W25QXX_BUF, secpos * 4096, 4096); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(W25QXX_BUF[secoff + i] != 0XFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            W25QXX2_Erase_Sector(secpos);//�����������
            for(i = 0; i < secremain; i++)	 //����
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX2_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //д����������

        }
        else W25QXX2_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        if(NumByteToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��
            NumByteToWrite -= secremain;				//�ֽ����ݼ�
            if(NumByteToWrite > 4096)secremain = 4096;	//��һ����������д����
            else secremain = NumByteToWrite;			//��һ����������д����
        }
    };
}

//��������оƬ
//�ȴ�ʱ�䳬��...
void W25QXX1_Erase_Chip(void)
{
    W25QXX1_Write_Enable();                  //SET WEL
    W25QXX1_Wait_Busy();
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_ChipErase);        //����Ƭ��������
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
    W25QXX1_Wait_Busy();   				   //�ȴ�оƬ��������
}
void W25QXX2_Erase_Chip(void)
{
    W25QXX2_Write_Enable();                  //SET WEL
    W25QXX2_Wait_Busy();
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_ChipErase);        //����Ƭ��������
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
    W25QXX2_Wait_Busy();   				   //�ȴ�оƬ��������
}
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void W25QXX1_Erase_Sector(uint32_t Dst_Addr)
{
    //����falsh�������,������
    //printf("fe:%x\r\n",Dst_Addr);
    Dst_Addr *= 4096;
    W25QXX1_Write_Enable();                  //SET WEL
    W25QXX1_Wait_Busy();
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_SectorErase);   //������������ָ��
    if(W25QXX_TYPE == W25Q256)              //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        send_byte_to_flash((uint8_t)((Dst_Addr) >> 24));
    }
    send_byte_to_flash((uint8_t)((Dst_Addr) >> 16)); //����24bit��ַ
    send_byte_to_flash((uint8_t)((Dst_Addr) >> 8));
    send_byte_to_flash((uint8_t)Dst_Addr);
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
    W25QXX1_Wait_Busy();   				    //�ȴ��������
}

void W25QXX2_Erase_Sector(uint32_t Dst_Addr)
{
    //����falsh�������,������
    //printf("fe:%x\r\n",Dst_Addr);
    Dst_Addr *= 4096;
    W25QXX2_Write_Enable();                  //SET WEL
    W25QXX2_Wait_Busy();
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_SectorErase);   //������������ָ��
    if(W25QXX_TYPE == W25Q256)              //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        send_byte_to_flash((uint8_t)((Dst_Addr) >> 24));
    }
    send_byte_to_flash((uint8_t)((Dst_Addr) >> 16)); //����24bit��ַ
    send_byte_to_flash((uint8_t)((Dst_Addr) >> 8));
    send_byte_to_flash((uint8_t)Dst_Addr);
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
    W25QXX2_Wait_Busy();   				    //�ȴ��������
}
//�ȴ�����
void W25QXX1_Wait_Busy(void)
{
    while((W25QXX1_ReadSR(1) & 0x01) == 0x01); // �ȴ�BUSYλ���
}
void W25QXX2_Wait_Busy(void)
{
    while((W25QXX2_ReadSR(1) & 0x01) == 0x01); // �ȴ�BUSYλ���
}
//�������ģʽ
void W25QXX1_PowerDown(void)
{
    W25QXX_CS1 = 0;                          //ʹ������
    send_byte_to_flash(W25X_PowerDown);     //���͵�������
    W25QXX_CS1 = 1;                          //ȡ��Ƭѡ
    //delay_us(3);                            //�ȴ�TPD
}
void W25QXX2_PowerDown(void)
{
    W25QXX_CS2 = 0;                          //ʹ������
    send_byte_to_flash(W25X_PowerDown);     //���͵�������
    W25QXX_CS2 = 1;                          //ȡ��Ƭѡ
    //delay_us(3);                            //�ȴ�TPD
}
//����
void W25QXX1_WAKEUP(void)
{
    W25QXX_CS1 = 0;                              //ʹ������
    send_byte_to_flash(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB
    W25QXX_CS1 = 1;                              //ȡ��Ƭѡ
    //delay_us(3);                                //�ȴ�TRES1
}
void W25QXX2_WAKEUP(void)
{
    W25QXX_CS2 = 0;                              //ʹ������
    send_byte_to_flash(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB
    W25QXX_CS2 = 1;                              //ȡ��Ƭѡ
    //delay_us(3);                                //�ȴ�TRES1
}

// ========== end of file

