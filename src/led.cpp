#include "../include/led.hpp"

LED::LED()
{
    leds.start();
}

void LED::Process(float deltaTime)
{
    counter += deltaTime;

    switch (state)
    {
    case LightState::Default:
    {
        if (counter >= speed)
        {
            offset += (float)speed / 1000.0f;
            for (auto i = 0u; i < servo2040::NUM_LEDS; i++)
            {
                float hue = (float)i / (float)servo2040::NUM_LEDS;
                leds.set_hsv(i, hue + offset, 1.0f, brightness);
            }
            counter = 0;
        }
        break;
    }

    case LightState::Error:
        // for (uint16_t i = 0; i < 6; i++)
        // {
        //     // leds.setPixelColor(i, 255, 0, 0);
        // }
        break;
    }
}

void LED::SetState(LightState state)
{
    this->state = state;
}