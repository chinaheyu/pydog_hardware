#include "cmsis_os.h"


extern void control_task_main(void *argument);


// control_task_mainΪcpp�ļ���Ϊ��ʵ�ֻ�ϱ��������extern "C"�����Ĵ�����control_task_main��
void control_task(void *argument)
{
    control_task_main(argument);
    
    while(1)
    {
        osDelay(100);
    }
}
