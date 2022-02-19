#include "quadruped_control.h"
#include <quadruped_description.h>

void QuadrupedControl::init(void)
{
    champ::URDF::loadFromHeader(base);
    
    joints_msg_.position_length = 12;
    contacts_msg_.contacts_length = 4;
    
    req_pose.orientation.roll = 0.0f;
    req_pose.orientation.pitch = 0.0f;
    req_pose.orientation.yaw = 0.0f;
    req_pose.position.z = 0.0f;
    
    req_vel.linear.x = 0.0f;
    req_vel.linear.y = 0.0f;
    req_vel.angular.z = 0.0f;
    
    nh->subscribe(vel_cmd_sub_);
    nh->subscribe(pose_cmd_sub_);
    nh->advertise(contacts_publisher_);
    nh->advertise(jointstates_publisher_);
    
    nh->loginfo("\033[1;32mQUADRUPED CONTROLLER INITIALIZED\033[0m");
}

void QuadrupedControl::update(float* out)
{
    req_pose.position.z = req_pose.position.z * gait_config.nominal_height;
    if(req_pose.position.z < 0.0)
    {
        req_pose.position.z = gait_config.nominal_height;
    }
    else if(req_pose.position.z < (gait_config.nominal_height * 0.5))
    {
        req_pose.position.z = gait_config.nominal_height * 0.5;
    }

    body_controller.poseCommand(target_foot_positions, req_pose);

    leg_controller.velocityCommand(target_foot_positions, req_vel);

    kinematics.inverse(target_joint_positions, target_foot_positions);
    
    for(int i = 0; i < 12; ++i)
    {
        out[i] = target_joint_positions[i];
    }
}

void QuadrupedControl::publishRobotState(void)
{
    // 发布关节
    joints_msg_.position = target_joint_positions;
    jointstates_publisher_.publish(&joints_msg_); 

    // 发布触地状态
    for(int i = 0; i < 4; i++)
    {
        if(base.legs[i]->gait_phase())
            foot_contacts[i] = 1;
        else
            foot_contacts[i] = 0;
    }
    contacts_msg_.contacts = foot_contacts;
    contacts_publisher_.publish(&contacts_msg_);
}

void QuadrupedControl::velocityCommandCallback(const geometry_msgs::Twist& vel_cmd_msg)
{
    req_vel.linear.x = vel_cmd_msg.linear.x;
    req_vel.linear.y = vel_cmd_msg.linear.y;
    req_vel.angular.z = vel_cmd_msg.angular.z;
}

void QuadrupedControl::poseCommandCallback(const champ_msgs::Pose& pose_cmd_msg)
{
    req_pose.orientation.roll = pose_cmd_msg.roll;
    req_pose.orientation.pitch = pose_cmd_msg.pitch;
    req_pose.orientation.yaw = pose_cmd_msg.yaw;
    req_pose.position.z = pose_cmd_msg.z;
}

