#include "bsp_usb.h"
#include "fifo.h"

extern fifo_s_t usb_rx_fifo;

/**
  * @brief          通过usb虚拟串口发送数据
  * @param[in]      data: 数据指针
  * @param[in]      len: 数据长度
  * @retval         none
  */
void usb_interface_send(uint8_t *data, uint16_t len)
{
    CDC_Transmit_FS(data, len);
}

/**
  * @brief          刷新rx缓冲区
  * @retval         none
  */
void usb_reset_rx_buf(void)
{
    fifo_s_flush(&usb_rx_fifo);
}

/**
  * @brief          read a byte from the serial port. -1 = failure
  * @param[in]      data: 数据指针
  * @param[in]      len: 数据长度
  * @retval         none
  */
int usb_rx_fifo_read(void)
{
    if(fifo_s_isempty(&usb_rx_fifo))
    {
        return -1;
    }
    return fifo_s_get(&usb_rx_fifo);
}
