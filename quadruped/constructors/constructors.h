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

#ifndef CONSTRUCTORS_H
#define CONSTRUCTORS_H

#include <actuators/actuator.h>
#include <gait_config.h>
#include <quadruped_base/quadruped_components.h>
#include <comms/input_interfaces/single_input_interface.h>
#include <comms/input_interfaces/dual_input_interface.h>
#include <comms/status_interfaces/status_interface.h>
#include <comms/input_interfaces/rosserial_interface.h>

#include <hardware_config.h>


// 定义驱动器
#include <actuators/digital_servo_plugin.h>

DigitalServo::Plugin lfh_actuator(LFH_PIN, -1.570796, 1.570796, LFH_OFFSET, LFH_INV);
DigitalServo::Plugin lfu_actuator(LFU_PIN, -1.570796, 1.570796, LFU_OFFSET, LFU_INV);
DigitalServo::Plugin lfl_actuator(LFL_PIN, -1.570796, 1.570796, LFL_OFFSET, LFL_INV);

DigitalServo::Plugin rfh_actuator(RFH_PIN, -1.570796, 1.570796, RFH_OFFSET, RFH_INV);
DigitalServo::Plugin rfu_actuator(RFU_PIN, -1.570796, 1.570796, RFU_OFFSET, RFU_INV);
DigitalServo::Plugin rfl_actuator(RFL_PIN, -1.570796, 1.570796, RFL_OFFSET, RFL_INV);

DigitalServo::Plugin lhh_actuator(LHH_PIN, -1.570796, 1.570796, LHH_OFFSET, LHH_INV);
DigitalServo::Plugin lhu_actuator(LHU_PIN, -1.570796, 1.570796, LHU_OFFSET, LHU_INV);
DigitalServo::Plugin lhl_actuator(LHL_PIN, -1.570796, 1.570796, LHL_OFFSET, LHL_INV);

DigitalServo::Plugin rhh_actuator(RHH_PIN, -1.570796, 1.570796, RHH_OFFSET, RHH_INV);
DigitalServo::Plugin rhu_actuator(RHU_PIN, -1.570796, 1.570796, RHU_OFFSET, RHU_INV);
DigitalServo::Plugin rhl_actuator(RHL_PIN, -1.570796, 1.570796, RHL_OFFSET, RHL_INV);

Actuator<DigitalServo::Plugin> actuators
(
    PANTOGRAPH_LEG,
    lfh_actuator, lfu_actuator, lfl_actuator,
    rfh_actuator, rfu_actuator, rfl_actuator,
    lhh_actuator, lhu_actuator, lhl_actuator,
    rhh_actuator, rhu_actuator, rhl_actuator
);


// 定义步态参数
champ::GaitConfig gait_config(
    KNEE_ORIENTATION,
    PANTOGRAPH_LEG,
    ODOM_SCALER,
    MAX_LINEAR_VELOCITY_X,
    MAX_LINEAR_VELOCITY_Y,
    MAX_ANGULAR_VELOCITY_Z,
    COM_X_TRANSLATION,
    SWING_HEIGHT,
    STANCE_DEPTH,
    STANCE_DURATION,
    NOMINAL_HEIGHT
);



// 遥控器及ROS输入
#include <comms/input_interfaces/rosserial_interface.h>
#include <comms/input_interfaces/rf_interface.h>

champ::Interfaces::ROSSerial ros_interface;
champ::Interfaces::RF rf_interface;

champ::Interfaces::Status<champ::Interfaces::ROSSerial> status_interface(ros_interface);
champ::Interfaces::DualInput<champ::Interfaces::ROSSerial, champ::Interfaces::RF> command_interface(ros_interface, rf_interface);


#endif