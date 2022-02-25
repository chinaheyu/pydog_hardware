#include "cmsis_os.h"
#include "ADS1115.h"
#include "log.h"


extern void publish_anemometer(float wind_direction, float wind_speed);
    
void anemometer_task(void *argument)
{
    float vol1;
    float vol2;

    // 开始循环读传感器
    while(1)
    {
        // 判断状态是否正常
        if(ADS1115_GetStatus())
        {
            ADS1115_AsyncReadADC(ADS1X15_REG_CONFIG_MUX_DIFF_0_1);
            osDelay(10);
            vol1 = ADS1115_GetAsyncResult();
            
            ADS1115_AsyncReadADC(ADS1X15_REG_CONFIG_MUX_DIFF_2_3);
            osDelay(10);
            vol2 = ADS1115_GetAsyncResult();
            
            // 把电压转化为数据
            float wing_speed = vol1 / 5.0f * 40.0f;
            float wing_direction = vol2 / 5.0f * 6.2831853f;
            
            publish_anemometer(wing_direction, wing_direction);
        }
        else
        {
            // 由于ADS1115离线时，HAL_I2C_IsDeviceReady会阻塞很长时间，所以补偿一个大延时，保障其他任务正常运行
            osDelay(1000);
        }
        
        osDelay(20);
    }
}
