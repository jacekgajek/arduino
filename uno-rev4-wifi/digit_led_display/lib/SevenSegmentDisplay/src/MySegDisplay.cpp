#include "MySegDisplay.h"

void MySegDisplay::refresh()
{
    setNumber(_number, _decPoint);
}

void MySegDisplay::storeInShiftRegister(uint8_t data)
{
    digitalWrite(shrRclk, LOW);
    for (byte i = 0; i < 8; i++)
    {
        digitalWrite(shrSrclk, LOW);
        digitalWrite(shrSer, data & (1 << i));
        digitalWrite(shrSrclk, HIGH);
    }
    digitalWrite(shrRclk, HIGH);
}

uint8_t MySegDisplay::setDigit(uint8_t whichDigit)
{
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
        if (i + 1 == whichDigit) {
            digitalWrite(digitPins[i], LOW);
        } else {
            digitalWrite(digitPins[i], HIGH);
        }
    }
}

uint8_t MySegDisplay::digitBits(uint8_t digit)
{
    switch (digit)
    {
    case 0:
        return 0b11111100;
    case 1:
        return 0b01100000;
    case 2:
        return 0b11011010;
    case 3:
        return 0b11110010;
    case 4:
        return 0b01100110;
    case 5:
        return 0b10110110;
    case 6:
        return 0b10111110;
    case 7:
        return 0b11100000;
    case 8:
        return 0b11111110;
    case 9:
        return 0b11110110;
    default:
        return 0;
    }
}

uint8_t MySegDisplay::pointBits()
{
    return 0b00000001;
}

void MySegDisplay::begin(pin_size_t ledDigits[DIGIT_COUNT], pin_size_t shrSer, pin_size_t shrRclk, pin_size_t shrSrclk)
{
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
        pinMode(ledDigits[i], OUTPUT);
    }

    pinMode(shrSer, OUTPUT);
    pinMode(shrRclk, OUTPUT);
    pinMode(shrSrclk, OUTPUT);

    for (int i = 0; i < DIGIT_COUNT; i++)
    {
        this->digitPins[i] = ledDigits[i];
    }
    
    this->shrSer = shrSer;
    this->shrSrclk = shrSrclk;
    this->shrRclk = shrRclk;
}

void MySegDisplay::setNumber(int number, int decimalPoint)
{
    // Serial.print("Setting number: ");
    // Serial.println(number);
    _number = number;
    _decPoint = decimalPoint;
    for (size_t i = 1; i <= DIGIT_COUNT; i++)
    {
        setDigit(0);
        // Serial.print(F("Setting digit "));
        // Serial.print(i);
        // Serial.print(F(" to "));
        // Serial.println(number % 10);
        auto bits = digitBits(number % 10);
        if (decimalPoint == i - 1)
        {
            bits |= pointBits();
        }
        storeInShiftRegister(bits);
        setDigit(DIGIT_COUNT - i + 1);
        number /= 10;
        delay(1);
    }
    setDigit(0);
}
