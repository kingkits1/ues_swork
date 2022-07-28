// File:fpga_manager.h
#ifndef __FPGE_MANAGER_H
#define __FPGE_MANAGER_H
typedef struct
{
    uint16_t nodefine0   :1;
    uint16_t hand_min    :1;
    uint16_t hand_max    :1;
    uint16_t min_close   :1;
    uint16_t max_close   :1;
    uint16_t _close      :1;
    uint16_t connect     :1;
    uint16_t nodefine78  :2;
    uint16_t dis_connect :1;
    uint16_t abcdef      :6;
}ST_FPGA_HANDLE_MESSAGE;

typedef struct
{   
    //电压值平方和
    uint32_t v2_sum;
    // 电流值的平方和
    uint32_t I2_sum;
    // 上升沿相位差 -90° -- +90°
    int8_t phase_up;
    // 下降沿相位差 -90° -- +90°
    int8_t phase_down;
    // 波峰相位差 -90° -- +90°
    int8_t phase_trough;
    // 波谷相位差 -90° -- +90°
    int8_t phase_peak;
    ST_FPGA_HANDLE_MESSAGE hand_msg;
    uint16_t xor_data_1_to_7;
    uint16_t xor_data_4_5;
    uint16_t data_available;
}ST_FPGA_DATA_DEFINES;

typedef union
{
    ST_FPGA_DATA_DEFINES data;
    uint16_t buf[10];
}UNION_FPGA_MSG;


void set_fpga_read_port(bool act);
void clear_fpga_data(void);
uint8_t fpga_read_data(void);

#endif
// End of File: fpga_manager.h

