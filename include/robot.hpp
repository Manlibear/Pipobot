#pragma once
#include <string>
#include <map>

class IRobot
{
public:
    virtual void SetJointAngle(std::string joint, float target);
    virtual void Process(float deltaTime);
    virtual void SetPulse(std::string joint, int target);
    virtual void StopMovement();
};
