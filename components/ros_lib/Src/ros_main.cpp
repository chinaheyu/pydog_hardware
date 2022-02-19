#include "ros.h"
#include "cmsis_os.h"
#include "bsp_led.h"

#include "quadruped_control.h"


/**
  * @brief          ros主函数，注意变量尽量定义在栈上，动态内存分配使用pvPortMalloc
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern "C" void ros_main(void *argument)
{
    ros::NodeHandle nh;
    
    nh.initNode();
    
    /* 连接rosserial_python */
    while(!nh.connected())
    {
        nh.spinOnce();
        osDelay(10);
    }
    
    nh.loginfo("\033[1;32mPYDOG HARDWARE CONNECTED\033[0m");
    
    // 构造四足控制器
    float joint_position[12];
    QuadrupedControl quadruped_control(nh);
    quadruped_control.init();
    
    uint32_t pre_control_time = time_us();
    uint32_t pre_publish_time = time_us();
    
    while (1)
    {
        /* 检测是否离线 */
        if(nh.connected())
        {
            led_red_off();
        }
        else
        {
            led_red_on();
        }
        
        // 控制频率100Hz
        if(time_us() - pre_control_time > 10000)
        {
            pre_control_time = time_us();
            quadruped_control.update(joint_position);
            // TODO 发送到舵机
        }
        
        // 状态发布频率50Hz
        if(time_us() - pre_publish_time > 20000)
        {
            pre_publish_time = time_us();
            quadruped_control.publishRobotState();
        }
        
        /* 发送所有数据 */
        nh.getHardware()->flush();
        
        /* 调用回调函数 */
        nh.spinOnce();
        
        /* 留给操作系统调度 */
        osDelay(2);
    }
}
