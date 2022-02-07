#include "bsp_key.h"
#include "gpio.h"


/**
  * @brief          判断KEY0是否按下
  * @retval         int
  */
int is_key0_pressed(void)
{
    return HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET;
}


/**
  * @brief          判断KEY_UP是否按下
  * @retval         int
  */
int is_key_up_pressed(void)
{
    return HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_SET;
}
