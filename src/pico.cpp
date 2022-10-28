#include <Arduino.h>
#include "../include/catbot.hpp"
#include "../include/serial_command.h"
#include "../include/led.hpp"
#include "../include/service_locator.hpp"

unsigned long lastTime;
std::shared_ptr<IRobot> catbot;
std::shared_ptr<ILights> lights;
std::shared_ptr<ICommandService> commands;

void setup()
{
    commands = ServiceLocator::Register<ICommandService>(new SerialCommandProcessor());
    lights = ServiceLocator::Register<ILights>(new LED());
    catbot = ServiceLocator::Register<IRobot>(new Catbot());

    lastTime = micros();
}

void loop()
{
    float deltaTime = (micros() - lastTime) / 1000.0f;
    lastTime = micros();

    commands->Process();
    catbot->Process(deltaTime);
    lights->Process(deltaTime);
}
