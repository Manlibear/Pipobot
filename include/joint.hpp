#pragma once

#pragma once
#include "../lib/servo2040/servo2040.hpp"
#include <ArduinoJson.h>
#include <vector>
#include <memory>
using namespace servo;

struct Joint
{
    std::string ID;
    float StartAngle;
    float CurrentAngle;
    float TargetAngle;
    Servo *ConnectedServo;
    int GPIOAddr;
    bool Inverted = false;
    char Key;
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

    template <>
    struct Converter<std::vector<std::pair<int, std::vector<Joint>>>>
    {
        static std::vector<std::pair<int, std::vector<Joint>>> fromJson(VariantConstRef src)
        {
            std::vector<std::pair<int, std::vector<Joint>>> result;

            for (auto json : src.as<JsonArrayConst>())
            {
                int length = json["Length"].as<int>();
                auto joints = json["Joints"].as<std::vector<Joint>>();
                result.push_back(std::make_pair(length, joints));
            }
            return result;
        }
    };

    template <>
    struct Converter<std::vector<std::pair<int, std::string>>>
    {
        static std::vector<std::pair<int, std::string>> fromJson(VariantConstRef src)
        {
            std::vector<std::pair<int, std::string>> result;
            for (auto json : src.as<JsonArrayConst>())
            {
                int length = json["Length"].as<int>();
                std::string joints = json["Joints"].as<std::string>();
                result.push_back(std::make_pair(length, joints));
            }
            return result;
        }
    };

    template <>
    struct Converter<std::vector<std::string>>
    {
        static std::vector<std::string> fromJson(VariantConstRef src)
        {
            std::vector<std::string> result;
            for (auto json : src.as<JsonArrayConst>())
            {
                std::string s = json.as<std::string>();
                result.push_back(s);
            }
            return result;
        }
    };
}