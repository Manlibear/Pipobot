#include "../include/catbot.hpp"
#include <Arduino.h>
#include <algorithm>
#include "../include/math_functions.h"

Catbot::Catbot()
{
    for (auto j : joints)
    {
        SetJointAngle(j.ID, j.CurrentAngle);
        j.CurrentAngle = j.TargetAngle;
        j.StartAngle = j.TargetAngle;
        j.ConnectedServo->attach(j.GPIOAddr);
        j.ConnectedServo->write(j.CurrentAngle);
    }
}

void Catbot::SetJointAngle(std::string joint, float target)
{
    for (auto it = joints.begin(); it != joints.end(); it++)
    {
        if (it->ID == joint)
        {
            if (it->Inverted)
                target = 180 - target;

            it->TargetAngle = target;
            break;
        }
    }
}

void Catbot::StopMovement()
{
    ElapsedTime = 0;
}

void Catbot::Process(float deltaTime)
{
    ElapsedTime += deltaTime;
    auto t = ElapsedTime / MovementTime;

    for (auto &j : joints)
    {
        if (floor(j.CurrentAngle) != floor(j.TargetAngle))
        {
            if (t >= 1)
                t = 1;

            auto newAngle = MathFunctions::Lerp(j.StartAngle, j.TargetAngle, MathFunctions::EaseInQuart(t));
            // auto newAngle = MathFunctions::Lerp(j.StartAngle, j.TargetAngle, t);

            if (floor(newAngle) != floor(j.CurrentAngle))
            {
                j.ConnectedServo->write(floor(newAngle));
                // Serial.println("*Moving " + String(j.ID.c_str()) + ": " + String(j.CurrentAngle) + "/" + String(j.TargetAngle));
            }

            j.CurrentAngle = newAngle;

            if (floor(j.CurrentAngle) == floor(j.TargetAngle))
            {
                j.StartAngle = j.TargetAngle;
            }
        }
    }
}

void Catbot::SetPulse(std::string joint, int target)
{
    for (auto it = joints.begin(); it != joints.end(); it++)
    {
        if (it->ID == joint)
        {
            Serial.println("Setting pulse of " + String(joint.c_str()) + " to " + String(target));
            it->ConnectedServo->writeMicroseconds(target);
        }
    }
}