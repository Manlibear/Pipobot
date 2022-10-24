#include <Arduino.h>
#include "../include/catbot.hpp"
#include "../include/serial_command.h"
#include "../include/service_locator.hpp"

std::shared_ptr<IRobot> catbot;
std::shared_ptr<ICommandService> commands;
unsigned long lastTime;
bool ledState = 0;
int counter = 0;


void setup()
{
    commands = ServiceLocator::Register<ICommandService>(new SerialCommandProcessor());
    catbot = ServiceLocator::Register<IRobot>(new Catbot());
    lastTime = millis();
    pinMode(25, OUTPUT);
}

void loop()
{
    float deltaTime = (micros() - lastTime) / 1000.0f;
    lastTime = micros();

    if(counter == 250000)
    {
        ledState = !ledState;
        digitalWrite(25, ledState);
        counter = 0;
    }
    counter++;
    commands->Process();
    catbot->Process(deltaTime);
}