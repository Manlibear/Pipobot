#pragma once
#include "robot.hpp"
#include "joint.hpp"
#include <vector>

class Catbot : public IRobot
{
public:
    Catbot();
    void SetJointAngle(std::string joint, float target);
    void Process(float deltaTime);
    void SetPulse(std::string joint, int target);
    void StopMovement();

private:
    Servo f_r_hip;
    Servo f_l_hip;
    Servo f_r_knee;
    Servo f_l_knee;
    Servo r_r_hip;
    Servo r_l_hip;
    Servo r_r_knee;
    Servo r_l_knee;

    const float MovementTime = 2000;
    float ElapsedTime = 0;

    std::vector<Joint> joints = {
        {"FrontRightHip", 0, 0, 0, &f_r_hip, 0, true},
        {"FrontRightKnee", 180, 0, 0, &f_r_knee, 1, true},

        {"FrontLeftHip", 0, 0, 0, &f_l_hip, 2},
        {"FrontLeftKnee", 180, 0, 0, &f_l_knee, 3},

        {"RearRightHip", 140, 0, 0, &r_r_hip, 12, true},
        {"RearRightKnee", 90, 0, 0, &r_r_knee, 13, true},

        {"RearLeftHip", 140, 0, 0, &r_l_hip, 14},
        {"RearLeftKnee", 90, 0, 0, &r_l_knee, 15},
    };
};