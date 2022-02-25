#include "cmsis_os.h"
#include "ADS1115.h"
#include "log.h"


extern void publish_anemometer(float wind_direction, float wind_speed);
    
void anemometer_task(void *argument)
{
    float vol1;
    float vol2;

    // ��ʼѭ����������
    while(1)
    {
        // �ж�״̬�Ƿ�����
        if(ADS1115_GetStatus())
        {
            ADS1115_AsyncReadADC(ADS1X15_REG_CONFIG_MUX_DIFF_0_1);
            osDelay(10);
            vol1 = ADS1115_GetAsyncResult();
            
            ADS1115_AsyncReadADC(ADS1X15_REG_CONFIG_MUX_DIFF_2_3);
            osDelay(10);
            vol2 = ADS1115_GetAsyncResult();
            
            // �ѵ�ѹת��Ϊ����
            float wing_speed = vol1 / 5.0f * 40.0f;
            float wing_direction = vol2 / 5.0f * 6.2831853f;
            
            publish_anemometer(wing_direction, wing_direction);
        }
        else
        {
            // ����ADS1115����ʱ��HAL_I2C_IsDeviceReady�������ܳ�ʱ�䣬���Բ���һ������ʱ����������������������
            osDelay(1000);
        }
        
        osDelay(20);
    }
}
