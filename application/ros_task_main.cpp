#include "ros.h"
#include "cmsis_os.h"
#include "bsp_led.h"

#include "quadruped_interface.h"

#include <olfaction_msgs/anemometer.h>
#include <olfaction_msgs/gas_sensor.h>

QuadrupedInterface* quadruped_interface_ptr;

olfaction_msgs::anemometer* anemometer_msg;
ros::Publisher* anemometer_publisher;

olfaction_msgs::gas_sensor* gas_sensor_msg;
ros::Publisher* gas_sensor_publisher;


extern "C" void publish_anemometer(float wind_direction, float wind_speed)
{
    if(anemometer_msg != NULL && anemometer_publisher != NULL)
    {
        anemometer_msg->wind_direction = wind_direction;
        anemometer_msg->wind_speed = wind_speed;

        anemometer_publisher->publish(anemometer_msg);
    }
}

extern "C" void publish_gas_sensor(float raw)
{
    if(gas_sensor_msg != NULL && gas_sensor_publisher != NULL)
    {
        gas_sensor_msg->raw = raw;
        gas_sensor_msg->raw_units = gas_sensor_msg->UNITS_PPM;

        gas_sensor_publisher->publish(gas_sensor_msg);
    }
}


/**
  * @brief          ros主函数，注意变量尽量定义在栈上，动态内存分配使用pvPortMalloc
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern "C" void ros_task_main(void *argument)
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
    
    QuadrupedInterface quadruped_interface(nh);
    quadruped_interface.init();
    quadruped_interface_ptr = &quadruped_interface;
    
    olfaction_msgs::anemometer anemometer_msg_;
    anemometer_msg = &anemometer_msg_;
    ros::Publisher anemometer_publisher_("anemometer/raw", &anemometer_msg_);
    anemometer_publisher = &anemometer_publisher_;
    nh.advertise(anemometer_publisher_);
    
    olfaction_msgs::gas_sensor gas_sensor_msg_;
    gas_sensor_msg = &gas_sensor_msg_;
    ros::Publisher gas_sensor_publisher_("gas_sensor/raw", &gas_sensor_msg_);
    gas_sensor_publisher = &gas_sensor_publisher_;
    nh.advertise(gas_sensor_publisher_);
    
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

        /* 发送所有数据 */
        nh.getHardware()->flush();
        
        /* 调用回调函数 */
        nh.spinOnce();
        
        /* 留给操作系统调度 */
        osDelay(5);
    }
}
