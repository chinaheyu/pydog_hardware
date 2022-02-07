#include "cmsis_os.h"
#include "ros_main.h"


void gas_sensor_task(void *argument)
{

    while (1)
    {
        // TODO: 读取传感器数据并调用回调函数
        gas_sensor_callback(1.0);
        osDelay(200);
    }
}
