#pragma once
#include <string>
#include <map>
#include "joint.hpp"

class IRobot
{
public:
    virtual void SetJointAngle(std::string joint, float target);
    virtual void Process(float deltaTime);
    virtual void SetPulse(std::string joint, int target);
    virtual void StopMovement();
    virtual void SetMovement(std::vector<std::pair<int, std::vector<Joint>>> frames);
    virtual std::string JointNameFromKey(char key);
};
