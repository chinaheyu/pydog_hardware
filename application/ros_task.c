#include "cmsis_os.h"


extern void ros_task_main(void *argument);


// ros_task_mainΪcpp�ļ���Ϊ��ʵ�ֻ�ϱ��������extern "C"�����Ĵ�����ros_task_main��
void ros_task(void *argument)
{
    ros_task_main(argument);
    
    while(1)
    {
        osDelay(100);
    }
}
