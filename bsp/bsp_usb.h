#ifndef BSP_USB_H
#define BSP_USB_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "usbd_cdc_if.h"

/**
  * @brief          ͨ��usb���⴮�ڷ�������
  * @param[in]      data: ����ָ��
  * @param[in]      len: ���ݳ���
  * @retval         none
  */
extern void usb_interface_send(uint8_t *data, uint16_t len);
   
/**
  * @brief          ˢ��tx��������������������
  * @retval         none
  */    
extern int32_t usb_tx_flush(void);

/**
  * @brief          ����rx������
  * @retval         none
  */
extern void usb_reset_rx_buf(void);

/**
  * @brief          ����tx������
  * @retval         none
  */
extern void usb_reset_tx_buf(void);

/**
  * @brief          read a byte from the serial port. -1 = failure
  * @param[in]      data: ����ָ��
  * @param[in]      len: ���ݳ���
  * @retval         none
  */
extern int usb_rx_fifo_read(void);

#ifdef  __cplusplus
}  
#endif

#endif
