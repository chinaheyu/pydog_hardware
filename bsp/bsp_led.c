#include "bsp_led.h"
#include "gpio.h"

/**
  * @brief          ����LED0����ɫ��
  * @retval         none
  */
void led_red_on(void)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
}


/**
  * @brief          Ϩ��LED0����ɫ��
  * @retval         none
  */
void led_red_off(void)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
}


/**
  * @brief          ��תLED0����ɫ��
  * @retval         none
  */
void led_red_toggle(void)
{
    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}


/**
  * @brief          ����LED1����ɫ��
  * @retval         none
  */
void led_green_on(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}


/**
  * @brief          Ϩ��LED1����ɫ��
  * @retval         none
  */
void led_green_off(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}


/**
  * @brief          ��תLED1����ɫ��
  * @retval         none
  */
void led_green_toggle(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}
