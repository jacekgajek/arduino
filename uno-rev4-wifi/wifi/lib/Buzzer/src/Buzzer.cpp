#include "Buzzer.h"

Buzzer::Buzzer(pin_size_t buzzerPin, BuzzerType buzzerType)
{
    pinMode(buzzerPin, OUTPUT);
    _buzzerPin = buzzerPin;
    _buzzerType = buzzerType;
}

void Buzzer::setDuration(int duration)
{
  _buzzDuration = duration;
}

void Buzzer::setDelay(int delay)
{
    _buzzDelay = delay;
}

void Buzzer::buzz()
{
    auto now = millis();
    if (_isActive && now - _prevBuzz > _buzzDelay)
    {
        if (!_buzzing) {
            _buzzStart = now;
        }
        if (now - _buzzStart <= _buzzDuration)
        {
            _buzzing = true;
            if (_buzzerType == PASSIVE) {
                tone(_buzzerPin, _tone);
            } else {
                digitalWrite(_buzzerPin, HIGH);
            }
        }
        else
        {
            _buzzing = false;
            if (_buzzerType == PASSIVE) {
                noTone(_buzzerPin);
            } else {
                digitalWrite(_buzzerPin, LOW);
            }
            _prevBuzz = now - _buzzDuration;
        }
    }
}

void Buzzer::activate()
{
    if (!_isActive) {
        _prevBuzz = millis();
    }
    _isActive = true;
}

void Buzzer::deactivate()
{
    _isActive = false;
}

bool Buzzer::isActive()
{
    return _isActive;
}

void Buzzer::setIsActive(bool isActive)
{
    if (isActive) {
        activate();
    } else {
        deactivate();
    }
}

void Buzzer::buzzNowWithSleep(u_int32_t millis)
{
    analogWrite(_buzzerPin, 255);
    delay(millis);
    analogWrite(_buzzerPin, 0);
}
