#include "cmsis_os.h"
#include "ros_main.h"


void gas_sensor_task(void *argument)
{

    while (1)
    {
        // TODO: ��ȡ���������ݲ����ûص�����
        gas_sensor_callback(1.0);
        osDelay(200);
    }
}
