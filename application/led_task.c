#include "bsp_led.h"
#include "cmsis_os.h"


/**
  * @brief          ��˸��ɫLED�Ʊ�ʾ������������
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
