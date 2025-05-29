#pragma once
#include <Arduino.h>
#include <Joystick.hpp>

class AnalogJoystick : public Joystick
{
private:
    int pinX, pinY, pinSw;

public:
    AnalogJoystick(int pinX, int pinY, int pinSw) : pinX(pinX), pinY(pinY), pinSw(pinSw)
    {
        pinMode(pinX, INPUT);
        pinMode(pinY, INPUT);
        pinMode(pinSw, INPUT);
    }

    JoystickState read()
    {
        int x = analogRead(pinX);
        int y = analogRead(pinY);
        int sw = analogRead(pinSw);
        if (sw < 10)
        {
            return CENTER;
        }
        else if (x < 300)
        {
            return LEFT;
        }
        else if (x > 700)
        {
            return RIGHT;
        }
        else if (y > 700)
        {
            return DOWN;
        }
        else if (y < 300)
        {
            return UP;
        }
        else
        {
            return NONE;
        }
    }
};
