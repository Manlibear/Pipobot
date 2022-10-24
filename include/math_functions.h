#pragma once
#include <algorithm>
#include <math.h>

class MathFunctions
{
public:
    static float Step(float &value, float target, float step)
    {
        return std::max(value - step, std::min(value + step, target));
    }

    static float Lerp(float a, float b, float f)
    {
        return a * (1.0 - f) + (b * f);
    }

    static float EaseInOutQuart(float x)
    {
        return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
    }

    static float EaseInQuart(float x){
        return x * x * x * x;
    }
};