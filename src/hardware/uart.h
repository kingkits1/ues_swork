
//File: Uart.h
#ifndef __USER_UART_H
#define __USER_UART_H
#define UART_TX_RX_BUF_LEN 128
#define UART_TX_RX_BUF_AND_VAL 0x7f
typedef enum
{
    EM_UART_PORT_1,
    EM_UART_PORT_2,
    EM_UART_PORT_3,
    EM_UART_PORT_4,
    EM_UART_PORT_5,
    EM_UART_PORT_6,
    EM_UART_PORT_7,
    EM_UART_PORT_8,
    EM_UART_PORT_default
} ENUM_UART_PORTS_DEFINES;
typedef struct
{
    uint8_t rxbuf[UART_TX_RX_BUF_LEN];
    uint8_t txbuf[UART_TX_RX_BUF_LEN];
    uint16_t rx_w_ptr;
    uint16_t rx_r_ptr;
    uint16_t tx_w_ptr;
    uint16_t tx_r_ptr;
} UART_DATA_DEFINES;

extern UART_DATA_DEFINES uart1_data;

void uartx_irq(uint8_t uart_port);
void init_uartx_data(UART_DATA_DEFINES *uartx_data);
void uart1_send_byte(uint8_t ch);
void uart1_send_string(uint8_t *str);



void init_uart_port_data(void);
void start_uart_port(void);

#endif

// end of file uart.h
