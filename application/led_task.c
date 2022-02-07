#include "bsp_led.h"
#include "cmsis_os.h"


/**
  * @brief          闪烁绿色LED灯表示程序正常运行
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void led_task(void *argument)
{

    while (1)
    {
        led_green_toggle();
        
        osDelay(200);
    }
}
