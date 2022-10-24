#pragma once

#include <Arduino.h>

class ICommandService
{
public:
    virtual void Process() = 0;
};
