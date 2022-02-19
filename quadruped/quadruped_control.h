#ifndef QUADRUPED_CONTROL_H
#define QUADRUPED_CONTROL_H

#include <gait_config.h>

#include <body_controller/body_controller.h>
#include <leg_controller/leg_controller.h>
#include <kinematics/kinematics.h>

#include <ros.h>

#include <champ_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <champ_msgs/Contacts.h>
#include <champ_msgs/Joints.h>


class QuadrupedControl
{
    public:
        QuadrupedControl(ros::NodeHandle& _nh): 
                            gait_config(KNEE_ORIENTATION,
                                        PANTOGRAPH_LEG,
                                        ODOM_SCALER,
                                        MAX_LINEAR_VELOCITY_X,
                                        MAX_LINEAR_VELOCITY_Y,
                                        MAX_ANGULAR_VELOCITY_Z,
                                        COM_X_TRANSLATION,
                                        SWING_HEIGHT,
                                        STANCE_DEPTH,
                                        STANCE_DURATION,
                                        NOMINAL_HEIGHT),
                            vel_cmd_sub_("cmd_vel/smooth", &QuadrupedControl::velocityCommandCallback, this),
                            pose_cmd_sub_("body_pose/raw", &QuadrupedControl::poseCommandCallback, this),
                            contacts_publisher_("foot_contacts/raw", &contacts_msg_),
                            jointstates_publisher_("joint_states/raw", &joints_msg_),
                            base(gait_config),
                            body_controller(base),
                            leg_controller(base),
                            kinematics(base),
                            nh(&_nh)
        {}
        
        // 初始化
        void init(void);
        
        // 更新并获取控制量控制舵机
        void update(float* out);

        // 发布状态
        void publishRobotState(void);

    private:
        ros::NodeHandle* nh;
        
        champ::GaitConfig gait_config;
    
        champ::QuadrupedBase base;
        champ::BodyController body_controller;
        champ::LegController leg_controller;
        champ::Kinematics kinematics;
    
        geometry::Transformation target_foot_positions[4];
    
        float target_joint_positions[12];
        bool foot_contacts[4];
    
        ros::Subscriber<geometry_msgs::Twist, QuadrupedControl> vel_cmd_sub_;
        ros::Subscriber<champ_msgs::Pose, QuadrupedControl> pose_cmd_sub_;
    
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


#endif // QUADRUPED_CONTROL_H
