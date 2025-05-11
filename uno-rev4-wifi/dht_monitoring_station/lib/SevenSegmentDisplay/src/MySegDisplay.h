#pragma once
#include "Arduino.h"

class MySegDisplay {

    private:
        static const int DIGIT_COUNT = 4;
        int _number;
        int _decPoint;
        pin_size_t digitPins[DIGIT_COUNT];
        pin_size_t outputEnablePin;
        uint8_t state[DIGIT_COUNT] = { 0 };

        pin_size_t shrRclk;

        void storeInShiftRegister(uint8_t data);
        void chooseDigit(int8_t whichDigit);
        uint8_t digitBits(uint8_t digit);
        uint8_t pointBits();
        void updateState(int8_t digit, uint8_t bits);

    public:
        void begin(pin_size_t ledDigits[DIGIT_COUNT], pin_size_t shrRclk, pin_size_t outputEnablePin);
        void setNumber(int number, int decimalPoint = -1);
        void setGlyph(int digit, uint8_t bits);
        void horizontalLine(int height);
        void refresh();
};
