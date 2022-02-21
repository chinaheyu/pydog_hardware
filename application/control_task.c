#include "cmsis_os.h"


extern void control_task_main(void *argument);


// control_task_main为cpp文件，为了实现混合编译进行了extern "C"，核心代码在control_task_main中
void control_task(void *argument)
{
    control_task_main(argument);
    
    while(1)
    {
        osDelay(100);
    }
}
