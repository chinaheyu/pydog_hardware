#include "cmsis_os.h"
#include "ros_main.h"


void anemometer_task(void *argument)
{

    while (1)
    {
        // TODO: 读取传感器数据并调用回调函数
        anemometer_callback(1.0, 2.0);
        osDelay(200);
    }
}
