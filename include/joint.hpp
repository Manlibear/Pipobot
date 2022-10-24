#pragma once

#pragma once
#include <Servo.h>
#include <ArduinoJson.h>
#include <vector>

struct Joint
{
    std::string ID;
    float StartAngle;
    float CurrentAngle;
    float TargetAngle;
    Servo *ConnectedServo;
    int GPIOAddr;
    bool Inverted = false;
    // ServoEaser Easer;

    bool operator()(const Joint &other) const
    {
        return other.ID == this->ID;
    }
};

namespace ARDUINOJSON_NAMESPACE
{
    template <>
    struct Converter<std::vector<Joint>>
    {
        static std::vector<Joint> fromJson(VariantConstRef src)
        {
            std::vector<Joint> result;

            for (auto json : src.as<JsonArrayConst>())
            {
                Joint j;
                j.ID = json["ID"].as<std::string>();
                j.TargetAngle = json["Angle"].as<float>();
                result.push_back(j);
            }

            return result;
        }
    };
}