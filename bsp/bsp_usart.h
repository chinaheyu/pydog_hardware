#ifndef BSP_USART_H
#define BSP_USART_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern void usart_printf(const char *fmt,...);
extern void usart_transmit(uint8_t *data, uint16_t len);

#ifdef  __cplusplus
}  
#endif

#endif
