#include "cmsis_os.h"
#include "ros_main.h"


// ros_mainΪcpp�ļ���Ϊ��ʵ�ֻ�ϱ��������extern "C"�����Ĵ�����ros_main��
void ros_task(void *argument)
{
    ros_main(argument);
}
