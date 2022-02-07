#include "bsp_key.h"
#include "gpio.h"


/**
  * @brief          �ж�KEY0�Ƿ���
  * @retval         int
  */
int is_key0_pressed(void)
{
    return HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET;
}


/**
  * @brief          �ж�KEY_UP�Ƿ���
  * @retval         int
  */
int is_key_up_pressed(void)
{
    return HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_SET;
}
