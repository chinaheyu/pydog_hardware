#include "bsp_led.h"
#include "cmsis_os.h"


/**
  * @brief          �����ʱ������˸��ɫLED�Ʊ�ʾ������������
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void led_blink(void *argument)
{
    led_green_toggle();
}
