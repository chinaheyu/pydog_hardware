#include "ros.h"
#include "cmsis_os.h"
#include "bsp_led.h"

#include "quadruped_control.h"


/**
  * @brief          ros��������ע���������������ջ�ϣ���̬�ڴ����ʹ��pvPortMalloc
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern "C" void ros_main(void *argument)
{
    ros::NodeHandle nh;
    
    nh.initNode();
    
    /* ����rosserial_python */
    while(!nh.connected())
    {
        nh.spinOnce();
        osDelay(10);
    }
    
    nh.loginfo("\033[1;32mPYDOG HARDWARE CONNECTED\033[0m");
    
    // �������������
    float joint_position[12];
    QuadrupedControl quadruped_control(nh);
    quadruped_control.init();
    
    uint32_t pre_control_time = time_us();
    uint32_t pre_publish_time = time_us();
    
    while (1)
    {
        /* ����Ƿ����� */
        if(nh.connected())
        {
            led_red_off();
        }
        else
        {
            led_red_on();
        }
        
        // ����Ƶ��100Hz
        if(time_us() - pre_control_time > 10000)
        {
            pre_control_time = time_us();
            quadruped_control.update(joint_position);
            // TODO ���͵����
        }
        
        // ״̬����Ƶ��50Hz
        if(time_us() - pre_publish_time > 20000)
        {
            pre_publish_time = time_us();
            quadruped_control.publishRobotState();
        }
        
        /* ������������ */
        nh.getHardware()->flush();
        
        /* ���ûص����� */
        nh.spinOnce();
        
        /* ��������ϵͳ���� */
        osDelay(2);
    }
}
