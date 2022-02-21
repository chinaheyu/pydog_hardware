#ifndef QUADRUPED_CONTROL_H
#define QUADRUPED_CONTROL_H

#include <gait_config.h>

#include <body_controller/body_controller.h>
#include <leg_controller/leg_controller.h>
#include <kinematics/kinematics.h>

class QuadrupedControl
{
    public:
        QuadrupedControl(): gait_config(KNEE_ORIENTATION,
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
                            base(gait_config),
                            body_controller(base),
                            leg_controller(base),
                            kinematics(base)
        {}
        
        // 初始化
        void init(void);
        
        // 更新控制量
        void update(champ::Velocities& req_vel, champ::Pose req_pose);

        // 获取状态
        void getJointState(bool* foot_contacts_out, float* target_joint_positions_out);

    private:
        champ::GaitConfig gait_config;

        champ::QuadrupedBase base;
        champ::BodyController body_controller;
        champ::LegController leg_controller;
        champ::Kinematics kinematics;

        geometry::Transformation target_foot_positions[4];

        float target_joint_positions[12];
        bool foot_contacts[4];
};


#endif // QUADRUPED_CONTROL_H
