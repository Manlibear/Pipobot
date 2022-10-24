#include "../include/serial_command.h"
#include "../include/robot.hpp"
#include "../include/joint.hpp"
#include <vector>
#include <ArduinoJson.h>
#include <Arduino.h>

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