#include "cmsis_os.h"
#include "bsp_key.h"
#include "log.h"

void key0_task(void *argument)
{
    key0_callback_flag = 1;
    while(1)
    {
        osEventFlagsWait(keyPressEventHandle, 0x00000001U, osFlagsWaitAny, osWaitForever);
        // TODO: 处理按键KEY0
        
        verbose_log("KEY0 is pressed.");

        osDelay(200);
        key0_callback_flag = 1;
    }
}

void key_up_task(void *argument)
{
    key_up_callback_flag = 1;
    while(1)
    {
        osEventFlagsWait(keyPressEventHandle, 0x00000002U, osFlagsWaitAny, osWaitForever);
        // TODO: 处理按键KEY_UP
        
        verbose_log("KEY_UP is pressed.");
        
        osDelay(200);
        key_up_callback_flag = 1;
    }
}

