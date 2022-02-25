#include "ros.h"
#include "cmsis_os.h"

#include "quadruped_control.h"
#include "quadruped_interface.h"
#include "hardware_config.h"

#include "pstwo.h"
#include "maestro.h"

#define RAD2DEG(x) (57.2957795130823208767981548141f * (x))

float inverse_array[12] = { LFH_INV, LFU_INV, LFL_INV,
                            RFH_INV, RFU_INV, RFL_INV,
                            LHH_INV, LHU_INV, LHL_INV,
                            RHH_INV, RHU_INV, RHL_INV};

float offset_array[12] = {  LFH_OFFSET, LFU_OFFSET, LFL_OFFSET,
                            RFH_OFFSET, RFU_OFFSET, RFL_OFFSET,
                            LHH_OFFSET, LHU_OFFSET, LHL_OFFSET,
                            RHH_OFFSET, RHU_OFFSET, RHL_OFFSET};

extern QuadrupedInterface* quadruped_interface_ptr;

inline bool check_interface(void)
{
    return quadruped_interface_ptr != NULL && quadruped_interface_ptr->is_connected();
}

extern "C" void control_task_main(void *argument)
{
    MAESTRO_Init();
    
    QuadrupedControl quadruped_controller;
    quadruped_controller.init();
    
    champ::Velocities req_vel;
    champ::Pose req_pose;
    
    bool foot_contacts[4];
    float target_joint_positions[12];
    uint16_t server_025us[12];

    uint32_t pre_publish_time = time_us();

    while (1)
    {
        // 获取指令
        if(RedLightMode)
        {
            req_vel.linear.x = 0.5f * (127.0f - (float)PS2_LY) / 255.0f;
            req_vel.linear.y = (float)(PS2_KEY & PSB_L1) * 0.5f * (128.0f - (float)PS2_LX) / 255.0f;
            req_vel.linear.z = 0.0f;
            req_vel.angular.x = 0.0f;
            req_vel.angular.y = 0.0f;
            req_vel.angular.z = (float)(!(PS2_KEY & PSB_L1)) * 1.0f * (128.0f - (float)PS2_LX) / 255.0f;
            
            req_pose.position.x = 0.0f;
            req_pose.position.y = 0.0f;
            req_pose.position.z = 0.0f;
            req_pose.orientation.roll = (float)(!(PS2_KEY & PSB_L2)) * ((float)PS2_RX - 128.0f) / 255.0f * 0.349066;
            req_pose.orientation.pitch = (127.0f - (float)PS2_RY) / 255.0f * 0.174533;
            req_pose.orientation.yaw = (float)(PS2_KEY & PSB_L2) * ((float)PS2_RX - 128.0f) / 255.0f * 0.436332;
        }
        else if(check_interface())
        {
            quadruped_interface_ptr->getCommand(req_vel, req_pose);
        }
        
        // 控制四足
        quadruped_controller.update(req_vel, req_pose);
        quadruped_controller.getJointState(foot_contacts, target_joint_positions);
        
        // 根据角度控制舵机
        for(int i = 0; i < 12; ++i)
        {
            server_025us[i] = ((RAD2DEG(inverse_array[i] * target_joint_positions[i] + offset_array[i]) + 90.0f) / 180.0f * 2000.0f + 500.0f) * 4;
        }
        MAESTRO_SetMultipleTargets(12, 0, server_025us);
        
        // 50Hz发布状态
        if(time_us() - pre_publish_time > 20000)
        {
            pre_publish_time = time_us();
            if(check_interface())
            {
                quadruped_interface_ptr->publishRobotState(foot_contacts, target_joint_positions);
            }
        }

        /* 留给操作系统调度 */
        osDelay(10);
    }
}
