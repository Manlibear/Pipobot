#pragma once

#include "lights.hpp"
#include "../lib/servo2040/servo2040.hpp"
using namespace plasma;
using namespace servo;
class LED : public ILights
{
private:
    WS2812 leds = WS2812(servo2040::NUM_LEDS, pio1, 0, servo2040::LED_DATA);
    const float brightness = 0.4f;
    LightState state = LightState::Default;
    float offset = 0;
    const uint updates = 50;
    float counter = 0;
    float speed = 5.0f;

public:
    LED();
    void Process(float deltaTime);
    void SetState(LightState state);
};