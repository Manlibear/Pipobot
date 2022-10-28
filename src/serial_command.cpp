#include "../include/serial_command.h"
#include "../include/robot.hpp"
#include "../include/joint.hpp"
#include <vector>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ctype.h>
#include <algorithm>
#include <utility>
#include "../include/string_helper.hpp"

SerialCommandProcessor::SerialCommandProcessor()
{
    Serial.begin(9600);
}

void SerialCommandProcessor::Process()
{
    if (Serial.available())
    {
        input = Serial.readString();
    }

    if (input.length() > 0)
    {
        try
        {
            DynamicJsonDocument command(1024);
            deserializeJson(command, input);
            Serial.println("Parsing");
            CommandType type = command["Type"];
            Serial.println("Got Type");
            switch (type)
            {
            case Pose:
            {
                Serial.println("Pose");
                auto bot = ServiceLocator::Resolve<IRobot>();
                bot->StopMovement();

                auto joints = command["Joints"].as<std::vector<Joint>>();

                Serial.println("Total Joints: " + String(joints.size()));
                for (auto j : joints)
                {
                    Serial.println("- " + String(j.ID.c_str()) + ": " + String(j.TargetAngle));
                    bot->SetJointAngle(j.ID, j.TargetAngle);
                }

                break;
            }
            case SetPulse:
            {
                Serial.println("SetPulse");
                auto bot = ServiceLocator::Resolve<IRobot>();
                auto joint = command["Joint"].as<std::string>();
                auto value = command["Value"].as<int>();

                bot->SetPulse(joint, value);
                break;
            }
            case Movement:
            {
                auto bot = ServiceLocator::Resolve<IRobot>();
                auto iterations = command["Iterations"].as<int>();
                auto frames = command["Frames"].as<std::vector<std::string>>();

                std::vector<std::pair<int, std::vector<Joint>>> movement;

                for (auto f : frames)
                {
                    if(f[0] == '#') // this frame is "commented out"
                        continue;
                    std::vector<std::string> parts = StringHelpers::Split(f, ",");
                    std::vector<Joint> joints;
                    std::string acc = "";
                    Joint j;
                    j.ID = "";

                    for (int i = 0; i < parts[1].size(); i++)
                    {
                        char c = parts[1][i];
                        if (isAlpha(c))
                        {
                            auto newID = bot->JointNameFromKey(c); //  TODO: precompute a lookup table
                            if (j.ID != newID && j.ID != "")
                            {
                                // we have one in the buffer, ready to store
                                j.TargetAngle = std::stof(acc);
                                joints.push_back(j);
                                acc = "";
                            }
                            j.ID = newID;
                        }
                        else
                        {
                            acc += c;
                        }
                    }

                    // and there's still one in the pipe, so store that now
                    j.TargetAngle = std::stof(acc);
                    joints.push_back(j);

                    movement.push_back(std::make_pair(std::stoi(parts[0]), joints));
                }

                Serial.println("Movement");
                Serial.println("Iterations: " + String(iterations));
                Serial.println("Frames: " + String(frames.size()));
                for (auto f : movement)
                {
                    Serial.println(String(f.first) + "ms");
                    for (auto j : f.second)
                    {
                        Serial.println(" - " + String(j.ID.c_str()) + " -> " + String(j.TargetAngle));
                    }
                }

                if (iterations > 1)
                {
                    auto originalSize = movement.size();
                    movement.reserve(iterations * originalSize);
                    for (int i = 0; i < iterations; i++)
                    {
                        std::copy_n(movement.begin(), originalSize, std::back_inserter(movement));
                    }
                }

                bot->SetMovement(movement);
                break;
            }
            case Unknown:
                Serial.println(" - Unknown command type: " + String((int)type));
                break;
            }
        }
        catch (const std::exception &e)
        {
            Serial.println(e.what());
        }

        input = "";
    }
}