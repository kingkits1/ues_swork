// File: Uart.c
#include "main.h"
#include "cmsis_os.h"
#include "../global.h"
#include "uart.h"
//==========
// 变量定义
//==========

extern UART_HandleTypeDef huart1;
UART_DATA_DEFINES uart1_data;
extern UART_HandleTypeDef huart3;
UART_DATA_DEFINES uart3_data;
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
        return;
    case EM_UART_PORT_3:
        // read
        while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE))
        {
            uart3_data.rxbuf[uart3_data.rx_w_ptr] = huart3.Instance->DR;
            uart3_data.rx_w_ptr ++;
            uart3_data.rx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        }

        // write
        if(uart3_data.tx_r_ptr != uart3_data.tx_w_ptr)
        {
            if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE))
            {
                huart3.Instance->DR = uart3_data.txbuf[uart3_data.tx_r_ptr];
                uart3_data.tx_r_ptr ++;
                uart3_data.tx_r_ptr &= UART_TX_RX_BUF_AND_VAL;
            }
        }
        else
        {
            //CLEAR_BIT(huart3.Instance->CR1, USART_CR1_TXEIE);
        }
        if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC))
        {
            CLEAR_BIT(huart3.Instance->CR1, USART_CR1_TXEIE);
        }
        return;
    case EM_UART_PORT_4:
    case EM_UART_PORT_5:
    case EM_UART_PORT_6:
        return;
    case EM_UART_PORT_7:
        return;
    case EM_UART_PORT_8:
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

void start_uart_port(void)
{
    SET_BIT(huart1.Instance->CR1, USART_CR1_RXNEIE);
}

// End of uart.c

