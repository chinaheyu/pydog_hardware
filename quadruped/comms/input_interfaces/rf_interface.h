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

#ifndef RF_INTERFACE_H
#define RF_INTERFACE_H

#include <quadruped_base/quadruped_components.h>
#include <macros/macros.h>
#include "pstwo.h"
#include <math.h>

// 摇杆死区
#define DEAD_BAND_WIDTH 5
#define DEAD_BAND_LOW (128-DEAD_BAND_WIDTH)
#define DEAD_BAND_HIGH (128+DEAD_BAND_WIDTH)

namespace champ
{
    namespace Interfaces
    {
        class RF
        {
            int lin_x_inv_, lin_y_inv_, ang_z_inv_, roll_inv_, pitch_inv_, yaw_inv_;

            champ::Velocities velocities_commands_;
            champ::Pose pose_commands_;

            unsigned long prev_vel_time_;
            unsigned long prev_pose_time_;
            unsigned long prev_poll_time_;

            bool vel_cmd_active_;
            bool pose_cmd_active_;
            public:
                RF():vel_cmd_active_(false), pose_cmd_active_(false)
                {
                    PS2_Init();
                }

                void invertAxes(bool linear_x, bool linear_y, bool angular_z, bool roll, bool pitch, bool yaw)
                {
                    lin_x_inv_ = (linear_x) ? -1 : 1;
                    lin_y_inv_ = (linear_y) ? -1 : 1;
                    ang_z_inv_ = (angular_z) ? -1 : 1;
                    roll_inv_ = (roll) ? -1 : 1;
                    pitch_inv_ = (pitch) ? -1 : 1;
                    yaw_inv_ = (yaw) ? -1 : 1;
                }

                void velocityInput(champ::Velocities &velocities)
                {
                    velocities = velocities_commands_;
                }

                void poseInput(champ::Pose &pose)
                {  
                    pose.orientation.roll = pose_commands_.orientation.roll;
                    pose.orientation.pitch = pose_commands_.orientation.pitch;
                    pose.orientation.yaw = pose_commands_.orientation.yaw;                
                }

                void jointsInput(float joints[12])
                {
     
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
                    return false;           
                }     

                void run()
                {
                    unsigned long now = time_us();
                    if((now - prev_poll_time_) > 500000)
                    {
                        prev_poll_time_ = now;

                        vel_cmd_active_ = false;
                        pose_cmd_active_ = false;
                        
                        // 读取手柄数据
                        PS2_Receive();
                        int rec_aux = PS2_RY;
                        int rec_ele = PS2_LY;
                        int rec_rud = PS2_LX;
                        int rec_ail = PS2_RX;
                        
                        // 没有姿态指令
                        pose_commands_.orientation.roll = 0.0f;
                        pose_commands_.orientation.pitch = 0.0f;
                        pose_commands_.orientation.yaw = 0.0f;
                        
                        pose_cmd_active_ = false;

                        // 速度指令
                        if(rec_rud > DEAD_BAND_HIGH || rec_rud < DEAD_BAND_LOW || rec_ail > DEAD_BAND_HIGH || rec_ail < DEAD_BAND_LOW || rec_ele > DEAD_BAND_HIGH || rec_ele < DEAD_BAND_LOW)
                        {
                            velocities_commands_.linear.x = mapFloat(rec_ele, 0, 256, -0.5, 0.5) * lin_x_inv_;
                            velocities_commands_.linear.y = mapFloat(rec_rud, 0, 256, -0.5, 0.5) * lin_y_inv_;
                            velocities_commands_.angular.z = mapFloat(rec_ail, 0, 256, -1.0, 1.0) * ang_z_inv_;
                            
                            vel_cmd_active_ = true;
                        }
                        else
                        {
                            velocities_commands_.linear.x = 0.0f;
                            velocities_commands_.linear.y = 0.0f;
                            velocities_commands_.angular.z = 0.0f;
                            
                            vel_cmd_active_ = false;
                        }

                    }
                }
        };
    } 
}

#endif 