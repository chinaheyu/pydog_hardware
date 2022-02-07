/*
Copyright (c) 2019-2020, Juan Miguel Jimeno
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ROSSERIAL_INTERFACE_H
#define ROSSERIAL_INTERFACE_H

#include <ros.h>
#include <ros/time.h>
#include <geometry/geometry.h>
#include <quadruped_base/quadruped_components.h>
#include <geometry_msgs/Twist.h>
#include <champ_msgs/Point.h>
#include <champ_msgs/PointArray.h>
#include <champ_msgs/Joints.h>
#include <champ_msgs/Pose.h>
#include <champ_msgs/Imu.h>
#include <champ_msgs/Velocities.h>
#include <champ_msgs/Contacts.h>
#include <macros/macros.h>

#include <olfaction_msgs/anemometer.h>
#include <olfaction_msgs/gas_sensor.h>

namespace champ
{
    namespace Interfaces
    {
        class ROSSerial
        {
            ros::NodeHandle nh_;

            ros::Subscriber<geometry_msgs::Twist, ROSSerial> vel_cmd_sub_;
            ros::Subscriber<champ_msgs::Pose, ROSSerial> pose_cmd_sub_;
            ros::Subscriber<champ_msgs::Joints, ROSSerial> joints_cmd_sub_;

            champ_msgs::Contacts contacts_msg_;
            ros::Publisher contacts_publisher_;

            champ_msgs::Joints joints_msg_;
            ros::Publisher jointstates_publisher_;
            
            olfaction_msgs::anemometer anemometer_msg_;
            ros::Publisher anemometer_publisher_;
            
            olfaction_msgs::gas_sensor gas_sensor_msg_;
            ros::Publisher gas_sensor_publisher_;

            // champ_msgs::Velocities vel_msg_;
            // ros::Publisher vel_publisher_;

            // champ_msgs::PointArray point_msg_;
            // ros::Publisher point_publisher_;

            champ::Velocities velocities_commands_;
            champ::Pose pose_commands_;
            float joints_commands_[12];

            unsigned long prev_vel_time_;
            unsigned long prev_pose_time_;
            unsigned long prev_joints_time_;
            unsigned long prev_resetter_time_;

            bool vel_cmd_active_;
            bool joints_cmd_active_;
            bool pose_cmd_active_;

            void velocityCommandCallback(const geometry_msgs::Twist& vel_cmd_msg)
            {
                prev_vel_time_ = time_us();
      
                velocities_commands_.linear.x = vel_cmd_msg.linear.x;
                velocities_commands_.linear.y = vel_cmd_msg.linear.y;
                velocities_commands_.angular.z = vel_cmd_msg.angular.z;
            }

            void poseCommandCallback(const champ_msgs::Pose& pose_cmd_msg)
            {
                prev_pose_time_ = time_us();

                pose_commands_.orientation.roll = pose_cmd_msg.roll;
                pose_commands_.orientation.pitch = pose_cmd_msg.pitch;
                pose_commands_.orientation.yaw = pose_cmd_msg.yaw;
                pose_commands_.position.z = pose_cmd_msg.z;
            }

            void jointsCommandCallback(const champ_msgs::Joints& joints_cmd_msg)
            {
                prev_joints_time_ = time_us();

                for(unsigned int i = 0; i < 12; i++)
                {
                    joints_commands_[i] = joints_cmd_msg.position[i];
                }
            }

            public:
                ROSSerial():
                    vel_cmd_sub_("cmd_vel/smooth", &ROSSerial::velocityCommandCallback, this),
                    pose_cmd_sub_("body_pose/raw", &ROSSerial::poseCommandCallback, this),
                    joints_cmd_sub_("cmd_joints", &ROSSerial::jointsCommandCallback, this),
                    contacts_publisher_("foot_contacts/raw", &contacts_msg_),
                    jointstates_publisher_("joint_states/raw", &joints_msg_),
                    anemometer_publisher_("anemometer/raw", &anemometer_msg_),
                    gas_sensor_publisher_("gas_sensor/raw", &gas_sensor_msg_),
                    // point_publisher_("foot/raw", &point_msg_),
                    // vel_publisher_("velocities/raw", &vel_msg_),
                    vel_cmd_active_(false),
                    joints_cmd_active_(false),
                    pose_cmd_active_(false)

                {
                    joints_msg_.position_length = 12;
                    contacts_msg_.contacts_length = 4;

                    nh_.initNode();

                    nh_.subscribe(vel_cmd_sub_);
                    nh_.subscribe(pose_cmd_sub_);
                    nh_.subscribe(joints_cmd_sub_);

                    nh_.advertise(contacts_publisher_);
                    nh_.advertise(jointstates_publisher_);
                    
                    nh_.advertise(anemometer_publisher_);
                    nh_.advertise(gas_sensor_publisher_);

                    // nh_.advertise(point_publisher_);
                    // nh_.advertise(vel_publisher_);

                    nh_.loginfo("PYDOG ROS CLIENT CONNECTED");
                }

                void velocityInput(champ::Velocities &velocities)
                {
                    velocities = velocities_commands_;
                }

                void poseInput(champ::Pose &pose)
                {  
                    pose = pose_commands_;
                }

                void jointsInput(float joints[12])
                {
                    for(unsigned int i = 0; i < 12; i++)
                    {
                        joints[i] = joints_commands_[i];
                    }            
                }

                bool velInputIsActive()
                {
                    return vel_cmd_active_;
                }

                bool poseInputIsActive()
                {       
                    return pose_cmd_active_;    
                }

                bool jointsInputIsActive()
                {
                    return joints_cmd_active_;
                }

                void run()
                {   
                    unsigned long now = time_us();
                    if((now- prev_resetter_time_) > 333333)
                    {
                        prev_resetter_time_ = now;
                        if((now - prev_vel_time_) < 500000)
                        {
                            vel_cmd_active_ = true;
                        }
                        else
                        {
                            vel_cmd_active_ = false;

                            velocities_commands_.linear.x = 0.0f;
                            velocities_commands_.linear.y = 0.0f;
                            velocities_commands_.angular.z = 0.0f;
                        }

                        if((now - prev_pose_time_) < 500000)
                        {
                            pose_cmd_active_ = true;
                        }
                        else
                        {   
                            pose_cmd_active_ = false;

                            pose_commands_.orientation.roll = 0.0f;
                            pose_commands_.orientation.pitch = 0.0f;
                            pose_commands_.orientation.yaw = 0.0f;
                        }

                        if((now - prev_joints_time_) < 500000)
                        {
                            joints_cmd_active_ = true;
                        }
                        else
                        {
                            joints_cmd_active_ = false;
                        }
                    }
                    nh_.spinOnce();
                }
                
                // void publishPoints(geometry::Transformation foot_positions[4])
                // {
                //     point_msg_.lf.x = foot_positions[0].X();
                //     point_msg_.lf.y = foot_positions[0].Y();
                //     point_msg_.lf.z = foot_positions[0].Z();

                //     point_msg_.rf.x = foot_positions[1].X();
                //     point_msg_.rf.y = foot_positions[1].Y();
                //     point_msg_.rf.z = foot_positions[1].Z();

                //     point_msg_.lh.x = foot_positions[2].X();
                //     point_msg_.lh.y = foot_positions[2].Y();
                //     point_msg_.lh.z = foot_positions[2].Z();

                //     point_msg_.rh.x = foot_positions[3].X();
                //     point_msg_.rh.y = foot_positions[3].Y();
                //     point_msg_.rh.z = foot_positions[3].Z();

                //     point_publisher_.publish(&point_msg_);
                // }

                // void publishVelocities(champ::Velocities vel)
                // {
                //     vel_msg_.linear_x = vel.linear.x;
                //     vel_msg_.linear_y = vel.linear.y;
                //     vel_msg_.angular_z = vel.angular.z;

                //     vel_publisher_.publish(&vel_msg_);
                // }
                
                void publishAnemometer(double wind_direction, double wind_speed)
                {
                    anemometer_msg_.wind_direction = wind_direction;
                    anemometer_msg_.wind_speed = wind_speed;

                    anemometer_publisher_.publish(&anemometer_msg_);
                }
                
                void publishGasSensor(double raw)
                {
                    gas_sensor_msg_.raw = raw;
                    gas_sensor_msg_.raw_units = gas_sensor_msg_.UNITS_PPM;

                    gas_sensor_publisher_.publish(&gas_sensor_msg_);
                }

                void publishJointStates(float joint_positions[12])
                {
                    joints_msg_.position = joint_positions;
                    jointstates_publisher_.publish(&joints_msg_);  
                }

                void publishFootContacts(bool foot_contacts[4])
                {
                    contacts_msg_.contacts = foot_contacts;

                    contacts_publisher_.publish(&contacts_msg_);
                }
        };
    }
    
}
#endif
