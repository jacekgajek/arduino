#include "Blinker.h"

Blinker::Blinker(pin_size_t ledPin)
{
    pinMode(ledPin, OUTPUT);
    _ledPin = ledPin;
}

void Blinker::setLedDelay(int delay)
{
    _delay = delay;
}

void Blinker::blink()
{
    if (!_isActive) {
        return;
    }
    int now = millis();

    if (now - _prevBlink >= _delay)
    {
        _prevBlink = now;
    }

    int half = _delay / 2;
    int quarter = _delay / 4;
    int part = now - _prevBlink;
    float ledValue;
    if (part < half)
    {
        if (part < quarter)
        {
            ledValue = 1.0f * part / quarter;
        }
        else
        {
            ledValue = -1.0f * (part - quarter) / quarter + 1.0f;
        }
    }
    else
    {
        ledValue = 0.0f;
    }

    analogWrite(_ledPin, (int)(ledValue * 255));
}

void Blinker::activate()
{
    if (!_isActive) {
        _prevBlink = 0;
    }
    _isActive = true;
}

void Blinker::deactivate()
{
    _isActive = false;
}

void Blinker::blinkNowWithSleep(u_int32_t millis)
{
    analogWrite(_ledPin, 255);
    delay(millis);
    analogWrite(_ledPin, 0);
}
