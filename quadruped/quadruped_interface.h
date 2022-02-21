#ifndef QUADRUPED_INTERFACE_H
#define QUADRUPED_INTERFACE_H


#include <ros.h>

#include <champ_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <champ_msgs/Contacts.h>
#include <champ_msgs/Joints.h>

#include <quadruped_base/quadruped_components.h>

class QuadrupedInterface
{
    public:
        QuadrupedInterface(ros::NodeHandle& _nh): 
                            vel_cmd_sub_("cmd_vel/smooth", &QuadrupedInterface::velocityCommandCallback, this),
                            pose_cmd_sub_("body_pose/raw", &QuadrupedInterface::poseCommandCallback, this),
                            contacts_publisher_("foot_contacts/raw", &contacts_msg_),
                            jointstates_publisher_("joint_states/raw", &joints_msg_),
                            nh(&_nh)
        {}
        
        // 是否连接
        bool is_connected(void);
        
        // 初始化
        bool init(void);

        // 获取命令
        void getCommand(champ::Velocities& vel_cmd, champ::Pose& pose_cmd);

        // 发布状态
        void publishRobotState(bool* foot_contacts, float* target_joint_positions);

    private:
        ros::NodeHandle* nh;
    
        ros::Subscriber<geometry_msgs::Twist, QuadrupedInterface> vel_cmd_sub_;
        ros::Subscriber<champ_msgs::Pose, QuadrupedInterface> pose_cmd_sub_;
    
        champ_msgs::Contacts contacts_msg_;
        ros::Publisher contacts_publisher_;

        champ_msgs::Joints joints_msg_;
        ros::Publisher jointstates_publisher_;
    
        // ROS Callbacks
        void velocityCommandCallback(const geometry_msgs::Twist& vel_cmd_msg);
        void poseCommandCallback(const champ_msgs::Pose& pose_cmd_msg);
    
        champ::Velocities req_vel;
        champ::Pose req_pose;

};


#endif // QUADRUPED_INTERFACE_H
