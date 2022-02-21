#include "quadruped_interface.h"

bool QuadrupedInterface::is_connected(void)
{
    return nh->connected();
}

bool QuadrupedInterface::init(void)
{
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
    
    nh->loginfo("\033[1;32mQUADRUPED INTERFACE INITIALIZED\033[0m");
}

void QuadrupedInterface::getCommand(champ::Velocities& vel_cmd, champ::Pose& pose_cmd)
{
    vel_cmd = req_vel;
    pose_cmd = req_pose;
}

void QuadrupedInterface::publishRobotState(bool* foot_contacts, float* target_joint_positions)
{
    // 发布关节
    joints_msg_.position = target_joint_positions;
    jointstates_publisher_.publish(&joints_msg_); 

    // 发布触地状态
    contacts_msg_.contacts = foot_contacts;
    contacts_publisher_.publish(&contacts_msg_);
}

void QuadrupedInterface::velocityCommandCallback(const geometry_msgs::Twist& vel_cmd_msg)
{
    req_vel.linear.x = vel_cmd_msg.linear.x;
    req_vel.linear.y = vel_cmd_msg.linear.y;
    req_vel.angular.z = vel_cmd_msg.angular.z;
}

void QuadrupedInterface::poseCommandCallback(const champ_msgs::Pose& pose_cmd_msg)
{
    req_pose.orientation.roll = pose_cmd_msg.roll;
    req_pose.orientation.pitch = pose_cmd_msg.pitch;
    req_pose.orientation.yaw = pose_cmd_msg.yaw;
    req_pose.position.z = pose_cmd_msg.z;
}

