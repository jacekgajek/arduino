#pragma once
#include <Arduino.h>

class Blinker {
    private:
    volatile int _delay = 3000;
    int _prevBlink = 0;
    bool _isActive = false;
    pin_size_t _ledPin = 0;
    pin_size_t _buzzerPin = 0;

    public:
    Blinker(pin_size_t ledPin);

    void setLedDelay(int delay);

    void blink();

    void activate();
    void deactivate();

    void blinkNowWithSleep(u_int32_t millis);
};

