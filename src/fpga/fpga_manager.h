// File:fpga_manager.h
#ifndef __FPGE_MANAGER_H
#define __FPGE_MANAGER_H
typedef enum
{
    EM_HANDLER_STATUS_MIN        = 1, // MIN按下
    EM_HANDLER_STATUS_MAX        = 2, // MAX按下
    EM_HANDLER_STATUS_MIN_CLOSE  = 3, // MIN按下且钳口闭合
    EM_HANDLER_STATUS_MAX_CLOSE  = 4, // MAX按下且钳口闭合
    EM_HANDLER_STATUS_ONLY_Close = 5, // 仅钳口闭合，MAX/MIN未按下，可能脚踏使用
    EM_HANDLER_STATUS_CONNECT    = 6, //手柄接通	6
    EM_HANDLER_STATUS_DISCONNECT = 9  //手柄断开	9
} ENUM_HANDLER_STATUS_DEFINES;

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

