#ifndef BSP_LED_H
#define BSP_LED_H

#ifdef  __cplusplus
extern "C" {
#endif
    
/**
  * @brief          ����LED0����ɫ��
  * @retval         none
  */
extern void led_red_on(void);


/**
  * @brief          Ϩ��LED0����ɫ��
  * @retval         none
  */
extern void led_red_off(void);


/**
  * @brief          ��תLED0����ɫ��
  * @retval         none
  */
extern void led_red_toggle(void);


/**
  * @brief          ����LED1����ɫ��
  * @retval         none
  */
extern void led_green_on(void);


/**
  * @brief          Ϩ��LED1����ɫ��
  * @retval         none
  */
extern void led_green_off(void);


/**
  * @brief          ��תLED1����ɫ��
  * @retval         none
  */
extern void led_green_toggle(void);

#ifdef  __cplusplus
}  
#endif

#endif
