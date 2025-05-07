#pragma once
#include <Arduino.h>

enum BuzzerType {
    ACTIVE, PASSIVE
};

class Buzzer {
    private:
    int _prevBuzz = 0;
    int _buzzStart = 0;
    bool _buzzing = false;
    bool _isActive = false;
    int _buzzDuration = 100;
    int _buzzDelay = 1000;
    int _tone = 440;
    BuzzerType _buzzerType = PASSIVE;
    pin_size_t _buzzerPin = 0;

    public:
    Buzzer(pin_size_t buzzerPin, BuzzerType buzzerType);

    void setDuration(int duration);
    void setDelay(int delay);

    void buzz();

    void activate();
    void deactivate();
    bool isActive();
    void setIsActive(bool isActive);

    void buzzNowWithSleep(u_int32_t millis);
};
