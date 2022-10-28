#pragma once

enum LightState
{
    Default,
    Error
};

class ILights
{
public:
    virtual void Process(float deltaTime);
    virtual void SetState(LightState state);
};
