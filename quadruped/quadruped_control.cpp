#include "quadruped_control.h"
#include <quadruped_description.h>

void QuadrupedControl::init(void)
{
    champ::URDF::loadFromHeader(base);
}

void QuadrupedControl::update(champ::Velocities& req_vel, champ::Pose req_pose)
{
    req_pose.position.z = req_pose.position.z * gait_config.nominal_height;
    if(req_pose.position.z <= 0.0)
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
    
    for(int i = 0; i < 4; i++)
    {
        if(base.legs[i]->gait_phase())
            foot_contacts[i] = 1;
        else
            foot_contacts[i] = 0;
    }
}

void QuadrupedControl::getJointState(bool* foot_contacts_out, float* target_joint_positions_out)
{
    int i;
    for(i = 0; i < 4; ++i)
    {
        foot_contacts_out[i] = foot_contacts[i];
    }
    for(i = 0; i < 12; ++i)
    {
        target_joint_positions_out[i] = target_joint_positions[i];
    }
    
}

