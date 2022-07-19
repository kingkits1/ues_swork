// File: Uart.c
#include "main.h"
#include "cmsis_os.h"
#include "../BDU_global.h"
#include "uart.h"
#include "blower.h"
#include "oxygen_concentration.h"
//#define MIN_BLOWER_SET_SPEED 5000
//==========
// 变量定义
//==========

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart8;

UART_DATA_DEFINES uart1_data;
UART_DATA_DEFINES uart2_data;
UART_DATA_DEFINES uart6_data;
UART_DATA_DEFINES uart8_data;


//==========
// 函数定义
//==========

void init_uartx_data(UART_DATA_DEFINES *uartx_data)
{
    uint16_t i;
    uartx_data->rx_r_ptr = 0;
    uartx_data->rx_w_ptr = 0;
    uartx_data->tx_r_ptr = 0;
    uartx_data->tx_w_ptr = 0;
    for(i = 0; i < UART_TX_RX_BUF_LEN; i++)
    {
        uartx_data->txbuf[i] = 0;
        uartx_data->rxbuf[i] = 0;
    }
}

void uartx_irq(uint8_t uart_port)
{
    switch(uart_port)
    {
    case EM_UART_PORT_1:
        // read
        while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
        {
            uart1_data.rxbuf[uart1_data.rx_w_ptr] = huart1.Instance->DR;
            uart1_data.rx_w_ptr ++;
            uart1_data.rx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        }

        // write
        if(uart1_data.tx_r_ptr != uart1_data.tx_w_ptr)
        {
            if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE))
            {
                huart1.Instance->DR = uart1_data.txbuf[uart1_data.tx_r_ptr];
                uart1_data.tx_r_ptr ++;
                uart1_data.tx_r_ptr &= UART_TX_RX_BUF_AND_VAL;
            }
        }
        else
        {
            //CLEAR_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
        }
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC))
        {
            CLEAR_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
        }
        return;
    case EM_UART_PORT_2:
        // read
        while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
        {
            uart2_data.rxbuf[uart2_data.rx_w_ptr] = huart2.Instance->DR;
            uart2_data.rx_w_ptr ++;
            uart2_data.rx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        }

        // write
        if(uart2_data.tx_r_ptr != uart2_data.tx_w_ptr)
        {
            if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE))
            {
                huart2.Instance->DR = uart2_data.txbuf[uart2_data.tx_r_ptr];
                uart2_data.tx_r_ptr ++;
                uart2_data.tx_r_ptr &= UART_TX_RX_BUF_AND_VAL;
            }
        }
        else
        {
            //CLEAR_BIT(huart2.Instance->CR1, USART_CR1_TXEIE);
        }
        if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC))
        {
            CLEAR_BIT(huart2.Instance->CR1, USART_CR1_TXEIE);
        }
        return;
    case EM_UART_PORT_3:
        return;
    case EM_UART_PORT_4:
    case EM_UART_PORT_5:
    case EM_UART_PORT_6:
        // read
        while(__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE))
        {
            uart6_data.rxbuf[uart6_data.rx_w_ptr] = huart6.Instance->DR;
            uart6_data.rx_w_ptr ++;
            uart6_data.rx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        }

        // write
        if(uart6_data.tx_r_ptr != uart6_data.tx_w_ptr)
        {
            if(__HAL_UART_GET_FLAG(&huart6, UART_FLAG_TXE))
            {
                huart6.Instance->DR = uart6_data.txbuf[uart6_data.tx_r_ptr];
                uart6_data.tx_r_ptr ++;
                uart6_data.tx_r_ptr &= UART_TX_RX_BUF_AND_VAL;
            }
        }
        else
        {
            //CLEAR_BIT(huart6.Instance->CR1, USART_CR1_TXEIE);
        }
        if(__HAL_UART_GET_FLAG(&huart6, UART_FLAG_TC))
        {
            CLEAR_BIT(huart6.Instance->CR1, USART_CR1_TXEIE);
        }
        return;
    case EM_UART_PORT_7:
        return;
    case EM_UART_PORT_8:
        // read
        while(__HAL_UART_GET_FLAG(&huart8, UART_FLAG_RXNE))
        {
            uart8_data.rxbuf[uart8_data.rx_w_ptr] = huart8.Instance->DR;
            uart8_data.rx_w_ptr ++;
            uart8_data.rx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        }

        // write
        if(uart8_data.tx_r_ptr != uart8_data.tx_w_ptr)
        {
            if(__HAL_UART_GET_FLAG(&huart8, UART_FLAG_TXE))
            {
                huart8.Instance->DR = uart8_data.txbuf[uart8_data.tx_r_ptr];
                uart8_data.tx_r_ptr ++;
                uart8_data.tx_r_ptr &= UART_TX_RX_BUF_AND_VAL;
            }
        }
        else
        {
            //CLEAR_BIT(huart8.Instance->CR1, USART_CR1_TXEIE);
        }
        if(__HAL_UART_GET_FLAG(&huart8, UART_FLAG_TC))
        {
            CLEAR_BIT(huart8.Instance->CR1, USART_CR1_TXEIE);
        }
    default:
        return;
    }
}

void uart1_send_byte(uint8_t ch)
{
    uart1_data.txbuf[uart1_data.tx_w_ptr] = ch;
    uart1_data.tx_w_ptr ++;
    uart1_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    SET_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
}

void uart1_send_string(uint8_t *str)
{
    while(*str)
    {
        uart1_data.txbuf[uart1_data.tx_w_ptr] = *str;
        str++;
        uart1_data.tx_w_ptr ++;
        uart1_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
}

void uart1_send_buf(uint8_t *buf, uint16_t len)
{
    while(len--)
    {
        uart1_data.txbuf[uart1_data.tx_w_ptr] = *buf;
        buf++;
        uart1_data.tx_w_ptr ++;
        uart1_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
}

void uart2_send_byte(uint8_t ch)
{
    uart2_data.txbuf[uart2_data.tx_w_ptr] = ch;
    uart2_data.tx_w_ptr ++;
    uart2_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    SET_BIT(huart2.Instance->CR1, USART_CR1_TXEIE);
}

void uart2_send_string(uint8_t *str)
{
    while(*str)
    {
        uart2_data.txbuf[uart2_data.tx_w_ptr] = *str;
        str++;
        uart2_data.tx_w_ptr ++;
        uart2_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart2.Instance->CR1, USART_CR1_TXEIE);
}

void uart2_send_buf(uint8_t *buf, uint16_t len)
{
    while(len--)
    {
        uart2_data.txbuf[uart2_data.tx_w_ptr] = *buf;
        buf++;
        uart2_data.tx_w_ptr ++;
        uart2_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart2.Instance->CR1, USART_CR1_TXEIE);
}


void uart6_send_byte(uint8_t ch)
{
    uart6_data.txbuf[uart6_data.tx_w_ptr] = ch;
    uart6_data.tx_w_ptr ++;
    uart6_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    SET_BIT(huart6.Instance->CR1, USART_CR1_TXEIE);
}

void uart6_send_string(uint8_t *str)
{
    while(*str)
    {
        uart6_data.txbuf[uart6_data.tx_w_ptr] = *str;
        str++;
        uart6_data.tx_w_ptr ++;
        uart6_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart6.Instance->CR1, USART_CR1_TXEIE);
}

void uart6_send_buf(uint8_t *buf, uint16_t len)
{
    while(len--)
    {
        uart6_data.txbuf[uart6_data.tx_w_ptr] = *buf;
        buf++;
        uart6_data.tx_w_ptr ++;
        uart6_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart6.Instance->CR1, USART_CR1_TXEIE);
}

void uart8_send_byte(uint8_t ch)
{
    uart8_data.txbuf[uart8_data.tx_w_ptr] = ch;
    uart8_data.tx_w_ptr ++;
    uart8_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    SET_BIT(huart8.Instance->CR1, USART_CR1_TXEIE);
}

void uart8_send_string(uint8_t *str)
{
    while(*str)
    {
        uart8_data.txbuf[uart8_data.tx_w_ptr] = *str;
        str++;
        uart8_data.tx_w_ptr ++;
        uart8_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart8.Instance->CR1, USART_CR1_TXEIE);
}

void uart8_send_buf(uint8_t *buf, uint16_t len)
{
    while(len--)
    {
        uart8_data.txbuf[uart8_data.tx_w_ptr] = *buf;
        buf++;
        uart8_data.tx_w_ptr ++;
        uart8_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
    }
    SET_BIT(huart8.Instance->CR1, USART_CR1_TXEIE);
}


void init_uart_port_data(void)
{
    init_uartx_data(&uart1_data);
    init_uartx_data(&uart2_data);
    init_uartx_data(&uart6_data);
    init_uartx_data(&uart8_data);


    Oxygen_concentration_init();
    init_foc_receive_data();
}

void start_uart_port(void)
{
    SET_BIT(huart1.Instance->CR1, USART_CR1_RXNEIE);
    SET_BIT(huart2.Instance->CR1, USART_CR1_RXNEIE);
    SET_BIT(huart6.Instance->CR1, USART_CR1_RXNEIE);
    SET_BIT(huart8.Instance->CR1, USART_CR1_RXNEIE);

    Oxygen_concentration_start();
}

// End of uart.c

