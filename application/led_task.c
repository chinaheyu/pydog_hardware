#include "bsp_led.h"
#include "cmsis_os.h"


/**
  * @brief          软件定时器，闪烁绿色LED灯表示程序正常运行
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void led_blink(void *argument)
{
    led_green_toggle();
}
