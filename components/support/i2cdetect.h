#ifndef __i2CDETECT_H__
#define __i2CDETECT_H__
#ifdef  __cplusplus
extern "C" {
#endif
    
    
#include "i2c.h"
    
/**
  * @brief          ���i2c�����ϵ������豸
  * @param[in]      hi2c_ptr: i2c���ָ��
  * @param[out]     out: ����ַ�����ַ
  * @retval         none
  */
void i2cdetect(I2C_HandleTypeDef* hi2c_ptr, char* out);
    
    
#ifdef  __cplusplus
}  
#endif
#endif
