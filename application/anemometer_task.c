#include "cmsis_os.h"
#include "ros_main.h"


void anemometer_task(void *argument)
{

    while (1)
    {
        // TODO: ��ȡ���������ݲ����ûص�����
        anemometer_callback(1.0, 2.0);
        osDelay(200);
    }
}
