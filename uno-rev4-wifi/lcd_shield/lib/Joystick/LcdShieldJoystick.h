#pragma once
#include <Arduino.h>

enum JoystickState
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER,
    NONE
};

class LcdShieldJoystick
{
private:
    int pin;

public:
    LcdShieldJoystick(int joystickPin) : pin(joystickPin)
    {
        pinMode(pin, INPUT);
    }

    JoystickState read()
    {
        int joy = analogRead(pin);
        if (joy == 623)
        {
            return LEFT;
        }
        else if (joy == 821)
        {
            return DOWN;
        }
        else if (joy == 407)
        {
            return UP;
        }
        else if (joy == 0)
        {
            return RIGHT;
        }
        else if (joy == 207)
        {
            return CENTER;
        }
        else
        {
            return NONE;
        }
    }
};