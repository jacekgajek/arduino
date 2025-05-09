#pragma once
#include <Arduino.h>

class RgbLed
{
private:
    pin_size_t redPin;
    pin_size_t greenPin;
    pin_size_t bluePin;
    float_t redValue = 0;
    float_t greenValue = 0;
    float_t blueValue = 0;

    const int MAX_VALUE = 255;

    int toAnalogOutput(float_t colorValue);

public:
    RgbLed(pin_size_t rPin, pin_size_t gPin, pin_size_t bPin);
    void setColor(float_t r, float_t g, float_t b);
    void clear();
    void fadeToColor(float_t r, float_t g, float_t b, int duration);
    void fadeOut(int duration);
    void setRed(float_t r);
    void setGreen(float_t g);
    void setBlue(float_t b);
};
