#include "../include/catbot.hpp"
#include <Arduino.h>
#include <algorithm>
#include "../include/math_functions.h"

Catbot::Catbot()
{
    servo::Calibration c = servo::Calibration();
    c.apply_two_pairs(1000, 2000, 0, 180);
    c.limit_to_calibration(false, false);

    for (auto &j : joints)
    {
        j.ConnectedServo = new Servo(j.GPIOAddr, c);
        j.ConnectedServo->init();
    }
}

void Catbot::SetJointAngle(std::string joint, float target)
{
    for (auto &j : joints)
    {
        if (j.ID == joint)
        {
            if (j.Inverted)
                target = 180 - target;

            j.TargetAngle = target;
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

    float t;

    if (keyframes.size() > 0)
    {
        t = ElapsedTime / (float)keyframes[0].first;
        if (keyframes[0].second.size() == 0)
        {
            if (t >= 1)
            {
                Serial.println("Finished wait frame");
                NextFrame();
                ElapsedTime = 0;
                return;
            }
            return; // this is a wait
        }
    }
    else
    {
        t = ElapsedTime / MovementTime;
    }

    bool changes = false;
    for (auto &j : joints)
    {
        if (floor(j.CurrentAngle) != floor(j.TargetAngle))
        {
            changes = true;
            if (t >= 1)
                t = 1;

            auto newAngle = MathFunctions::Lerp(j.StartAngle, j.TargetAngle, MathFunctions::EaseInQuart(t));
            // auto newAngle = MathFunctions::Lerp(j.StartAngle, j.TargetAngle, t);

            if (floor(newAngle) != floor(j.CurrentAngle))
            {
                j.ConnectedServo->value(floor(newAngle));
                // Serial.println("*Moving " + String(j.ID.c_str()) + ": " + String(j.CurrentAngle) + "/" + String(j.TargetAngle));
            }

            j.CurrentAngle = newAngle;

            if (floor(j.CurrentAngle) == floor(j.TargetAngle))
            {
                j.StartAngle = j.TargetAngle;
            }
        }
    }

    if (keyframes.size() > 0 && !changes)
    {
        // looks like we have a frame to play that we're already at, so let's go to the next one
        NextFrame();
    }
}

void Catbot::SetPulse(std::string joint, int target)
{
    for (auto it = joints.begin(); it != joints.end(); it++)
    {
        if (it->ID == joint)
        {
            Serial.println("Setting pulse of " + String(joint.c_str()) + " to " + String(target));
            it->ConnectedServo->pulse((float)target);
        }
    }
}

void Catbot::SetMovement(std::vector<std::pair<int, std::vector<Joint>>> frames)
{
    ElapsedTime = 0;
    keyframes.clear();
    keyframes = frames;
    if (keyframes.size() > 0)
    {
        Serial.println("Movement frames set");
        for (auto j : keyframes[0].second)
        {
            SetJointAngle(j.ID, j.TargetAngle);
        }
    }
}

void Catbot::NextFrame()
{
    keyframes.erase(keyframes.begin());
    if (keyframes.size() > 0)
    {
        Serial.println("Switching to next frame");
        for (auto j : keyframes[0].second)
        {
            SetJointAngle(j.ID, j.TargetAngle);
        }
    }
    else
    {
        Serial.println("End of movement");
    }
    ElapsedTime = 0;
}

std::string Catbot::JointNameFromKey(char key)
{
    for (auto& j : joints)
    {
        if (j.Key == key)
            return j.ID;
    }
    return "NULL";
}