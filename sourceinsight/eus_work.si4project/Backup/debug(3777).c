#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"


uint8_t debug_printf_flag;
extern UART_HandleTypeDef huart1;
#ifdef DEBUG
uint16_t debug_flag;
static uint16_t debug_dat;
#endif

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
PUTCHAR_PROTOTYPE
{
    if(debug_printf_flag == 0)
    {
        /* 给USART写一个字符 */
        uart1_data.txbuf[uart1_data.tx_w_ptr] = ch;
        uart1_data.tx_w_ptr ++;
        uart1_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        SET_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
        return ch;
    }
    else
    {
        /* 循环直到发送完成 */
        while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
        /* 给USART写一个字符 */
        huart1.Instance->DR = ch;
        SET_BIT(huart1.Instance->CR1, USART_CR1_TE);
        /* 循环直到发送完成 */
        while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
        return ch;
    }
}
#else
int fputc(int ch, FILE *f)
{
    if(debug_printf_flag == 0)
    {
        /* 给USART写一个字符 */
        uart1_data.txbuf[uart1_data.tx_w_ptr] = ch;
        uart1_data.tx_w_ptr ++;
        uart1_data.tx_w_ptr &= UART_TX_RX_BUF_AND_VAL;
        SET_BIT(huart1.Instance->CR1, USART_CR1_TXEIE);
        return ch;
    }
    else
    {
        /* 循环直到发送完成 */
        while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
        /* 给USART写一个字符 */
        huart1.Instance->DR = ch;
        SET_BIT(huart1.Instance->CR1, USART_CR1_TE);
        /* 循环直到发送完成 */
        while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
        return ch;
    }
}

#endif

