#include "RgbLed.h"

int RgbLed::toAnalogOutput(float_t colorValue)
{
    return colorValue * MAX_VALUE;
}

RgbLed::RgbLed(pin_size_t rPin, pin_size_t gPin, pin_size_t bPin)
{
    redPin = rPin;
    greenPin = gPin;
    bluePin = bPin;
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void RgbLed::setColor(float_t r, float_t g, float_t b)
{
    redValue = constrain(r, 0, 1);
    greenValue = constrain(g, 0, 1);
    blueValue = constrain(b, 0, 1);
    analogWrite(redPin, toAnalogOutput(redValue));
    analogWrite(greenPin, toAnalogOutput(greenValue));
    analogWrite(bluePin, toAnalogOutput(blueValue));
}

void RgbLed::clear()
{
    setColor(0, 0, 0);
}

void RgbLed::fadeToColor(float_t r, float_t g, float_t b, int duration)
{
    int steps = 100;
    float_t prevRedValue = redValue;
    float_t prevGreenValue = greenValue;
    float_t prevBlueValue = blueValue;

    float_t rStep = (r - redValue) / steps;
    float_t gStep = (g - greenValue) / steps;
    float_t bStep = (b - blueValue) / steps;
    for (int i = 0; i < steps; i++)
    {
        setColor(prevRedValue + rStep * i, prevGreenValue + gStep * i, prevBlueValue + bStep * i);
        delay(duration / steps);
    }
    setColor(r, g, b);
}

void RgbLed::fadeOut(int duration)
{
    fadeToColor(0, 0, 0, duration);
}

void RgbLed::setRed(float_t r)
{
    redValue = constrain(r, 0, 1);
    analogWrite(redPin, toAnalogOutput(redValue));
}

void RgbLed::setGreen(float_t g)
{
    greenValue = constrain(g, 0, 1);
    analogWrite(greenPin, toAnalogOutput(greenValue));
}

void RgbLed::setBlue(float_t b)
{
    blueValue = constrain(b, 0, 1);
    analogWrite(bluePin, toAnalogOutput(blueValue));
}
