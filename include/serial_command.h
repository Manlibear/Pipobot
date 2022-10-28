#pragma once
#include "command_service.h"
#include "service_locator.hpp"

class SerialCommandProcessor : public ICommandService
{
private:
    bool stringComplete;
    String input;

    enum CommandType
    {
        Unknown,
        Pose,
        SetPulse,
        Movement
    };

public:
    SerialCommandProcessor();
    void Process();
};