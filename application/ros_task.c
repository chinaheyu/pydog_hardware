#include "cmsis_os.h"
#include "ros_main.h"


// ros_main为cpp文件，为了实现混合编译进行了extern "C"，核心代码在ros_main中
void ros_task(void *argument)
{
    ros_main(argument);
}
