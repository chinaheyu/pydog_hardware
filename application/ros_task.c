#include "cmsis_os.h"


extern void ros_task_main(void *argument);


// ros_task_main为cpp文件，为了实现混合编译进行了extern "C"，核心代码在ros_task_main中
void ros_task(void *argument)
{
    ros_task_main(argument);
    
    while(1)
    {
        osDelay(100);
    }
}
