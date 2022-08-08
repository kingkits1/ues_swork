// File:fpga_manager.h
#ifndef __FPGE_MANAGER_H
#define __FPGE_MANAGER_H
typedef enum
{
    EM_HANDLER_STATUS_MIN        = 1, // MIN����
    EM_HANDLER_STATUS_MAX        = 2, // MAX����
    EM_HANDLER_STATUS_MIN_CLOSE  = 3, // MIN������ǯ�ڱպ�
    EM_HANDLER_STATUS_MAX_CLOSE  = 4, // MAX������ǯ�ڱպ�
    EM_HANDLER_STATUS_ONLY_Close = 5, // ��ǯ�ڱպϣ�MAX/MINδ���£����ܽ�̤ʹ��
    EM_HANDLER_STATUS_CONNECT    = 6, //�ֱ���ͨ	6
    EM_HANDLER_STATUS_DISCONNECT = 9  //�ֱ��Ͽ�	9
} ENUM_HANDLER_STATUS_DEFINES;

typedef struct
{
    //��ѹֵƽ����
    uint32_t v2_sum;
    // ����ֵ��ƽ����
    uint32_t I2_sum;
    // ��������λ�� -90�� -- +90��
    int8_t phase_up;
    // �½�����λ�� -90�� -- +90��
    int8_t phase_down;
    // ������λ�� -90�� -- +90��
    int8_t phase_trough;
    // ������λ�� -90�� -- +90��
    int8_t phase_peak;
    uint16_t hand_msg;
    uint16_t xor_data_1_to_7;
    uint16_t xor_data_4_5;
    uint16_t data_available;
} ST_FPGA_DATA_DEFINES;

typedef union
{
    ST_FPGA_DATA_DEFINES data;
    uint16_t buf[10];
} UNION_FPGA_MSG;


void set_fpga_read_port(bool act);
void clear_fpga_data(void);
uint8_t fpga_read_data(void);
void fpga_read_buf(uint16_t *pBuffer, uint32_t ReadAddr, uint32_t len);
void fpga_write_buf(uint16_t *pBuffer, uint32_t WriteAddr, uint32_t len);

#endif
// End of File: fpga_manager.h

