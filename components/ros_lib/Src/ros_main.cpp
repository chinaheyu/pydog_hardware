#include "ros_main.h"

#include "ros.h"
#include "cmsis_os.h"

#include <hardware_config.h>
#include <quadruped_description.h>
#include <gait_config.h>

#include <body_controller/body_controller.h>
#include <leg_controller/leg_controller.h>
#include <kinematics/kinematics.h>

#include <constructors/constructors.h>

champ::QuadrupedBase base(gait_config);
champ::BodyController body_controller(base);
champ::LegController leg_controller(base);
champ::Kinematics kinematics(base);

/**
  * @brief          ros主函数
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void ros_main(void *argument)
{
    champ::URDF::loadFromHeader(base);
    
    static unsigned long prev_publish_time = 0;
    
    geometry::Transformation target_foot_positions[4];
    float target_joint_positions[12]; 
    
    while (1)
    {
        champ::Pose req_pose;
        command_interface.poseInput(req_pose);
        req_pose.position.z = req_pose.position.z * gait_config.nominal_height;
        if(req_pose.position.z == 0.0)
        {
            req_pose.position.z = gait_config.nominal_height;
        }
        else if(req_pose.position.z < (gait_config.nominal_height * 0.5))
        {
            req_pose.position.z = gait_config.nominal_height * 0.5;
        }
   
        body_controller.poseCommand(target_foot_positions, req_pose);

        champ::Velocities req_vel;
        command_interface.velocityInput(req_vel);
        leg_controller.velocityCommand(target_foot_positions, req_vel);

        kinematics.inverse(target_joint_positions, target_foot_positions);

        command_interface.jointsInput(target_joint_positions);
        actuators.moveJoints(target_joint_positions);
        
        // 20ms发布一次状态
        if ((time_us() - prev_publish_time) >= 20000)
        {
            prev_publish_time = time_us();
            
            bool foot_contacts[4];
            float current_joint_positions[12];

            for(int i = 0; i < 4; i++)
            {
                if(base.legs[i]->gait_phase())
                    foot_contacts[i] = 1;
                else
                    foot_contacts[i] = 0;
            }
            status_interface.publishFootContacts(foot_contacts);

            actuators.getJointPositions(current_joint_positions);
            status_interface.publishJointStates(current_joint_positions);
        }
        
        command_interface.run();
        
        osDelay(2);
    }
}

void gas_sensor_callback(double raw)
{
    ros_interface.publishGasSensor(raw);
}

void anemometer_callback(double wind_direction, double wind_speed)
{
    ros_interface.publishAnemometer(wind_direction, wind_speed);
}
