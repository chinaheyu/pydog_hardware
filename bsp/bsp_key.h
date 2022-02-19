#ifndef BSP_KEY_H
#define BSP_KEY_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "gpio.h"
#include "cmsis_os.h"


extern osEventFlagsId_t keyPressEventHandle;
extern int key0_callback_flag;
extern int key_up_callback_flag;


#ifdef  __cplusplus
}  
#endif

#endif
