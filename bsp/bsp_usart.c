#include "bsp_usart.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>


static uint8_t usart_buf[512];


/**
  * @brief          通过usart1打印调试信息
  * @param[in]      类似printf
  * @retval         none
  */
void usart_printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)usart_buf, fmt, ap);

    va_end(ap);


    usart_transmit(usart_buf, len);
}

/**
  * @brief          通过usart1发送数据，不阻塞
  * @param[in]      data: 数据指针
  * @param[in]      len: 数据长度
  * @retval         none
  */
void usart_transmit(uint8_t *data, uint16_t len)
{
    
    HAL_UART_Transmit_DMA(&huart1, data, len);
}
