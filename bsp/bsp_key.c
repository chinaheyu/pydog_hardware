#include "bsp_key.h"


int key0_callback_flag;
int key_up_callback_flag;

extern void ads_finish_conversion_callback(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY0_Pin)
    {
        // KEY0被按下
        if(key0_callback_flag)
        {
            key0_callback_flag = 0;
            osEventFlagsSet(keyPressEventHandle, 0x00000001U);
        }
    }
    
    if(GPIO_Pin == KEY_UP_Pin)
    {
        // KEY_UP被按下
        if(key_up_callback_flag)
        {
            key_up_callback_flag = 0;
            osEventFlagsSet(keyPressEventHandle, 0x00000002U);
        }
    }
}
