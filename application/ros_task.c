#include "cmsis_os.h"


extern void ros_main(void *argument);


// ros_mainΪcpp�ļ���Ϊ��ʵ�ֻ�ϱ��������extern "C"�����Ĵ�����ros_main��
void ros_task(void *argument)
{
    ros_main(argument);
    
    while(1)
    {
        osDelay(100);
    }
}
