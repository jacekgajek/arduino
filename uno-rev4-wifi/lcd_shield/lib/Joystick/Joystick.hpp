#pragma once

enum JoystickState
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER,
    NONE
};

class Joystick {
public:
    virtual JoystickState read();
    virtual ~Joystick() = default;
};
