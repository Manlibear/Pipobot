#pragma once
#include "robot.hpp"
#include <vector>
#include <memory>
using namespace servo;

class Catbot : public IRobot
{
public:
    Catbot();
    void SetJointAngle(std::string joint, float target);
    void Process(float deltaTime);
    void SetPulse(std::string joint, int target);
    void StopMovement();
    void SetMovement(std::vector<std::pair<int, std::vector<Joint>>> frames);
    std::string JointNameFromKey(char key);

private:
    std::unique_ptr<Servo> f_r_hip;
    std::unique_ptr<Servo> f_l_hip;
    std::unique_ptr<Servo> f_r_knee;
    std::unique_ptr<Servo> f_l_knee;
    std::unique_ptr<Servo> r_r_hip;
    std::unique_ptr<Servo> r_l_hip;
    std::unique_ptr<Servo> r_r_knee;
    std::unique_ptr<Servo> r_l_knee;

    const float MovementTime = 1000;
    float ElapsedTime = 0;

    std::vector<Joint> joints = {
        {"FrontRightHip", 0, 0, 0, nullptr, 0, true, 'A'},
        {"FrontRightKnee", 180, 0, 0, nullptr, 1, true, 'B'},
        {"FrontLeftHip", 0, 0, 0, nullptr, 2, false, 'C'},
        {"FrontLeftKnee", 180, 0, 0, nullptr, 3, false, 'D'},
        {"RearRightHip", 140, 0, 0, nullptr, 4, true, 'E'},
        {"RearRightKnee", 90, 0, 0, nullptr, 5, true, 'F'},
        {"RearLeftHip", 140, 0, 0, nullptr, 6, true, 'G'},
        {"RearLeftKnee", 90, 0, 0, nullptr, 7, true, 'H'},
    };

    std::vector<std::pair<int, std::vector<Joint>>> keyframes;

    void NextFrame();
};