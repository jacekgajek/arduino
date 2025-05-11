#include "MySegDisplay.h"
#include "SPI.h"

void MySegDisplay::refresh()
{
    chooseDigit(-1);
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
        chooseDigit(i);
        storeInShiftRegister(state[i]);
        delay(1);
    }
    chooseDigit(-1);
}

void MySegDisplay::storeInShiftRegister(uint8_t data)
{
    digitalWrite(shrRclk, LOW);
    SPI.beginTransaction(SPISettings(20'000'000, BitOrder::LSBFIRST, SPI_MODE0));
    SPI.transfer(data);
    SPI.endTransaction();
    digitalWrite(shrRclk, HIGH);
}

void MySegDisplay::chooseDigit(int8_t whichDigit)
{
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
        digitalWrite(digitPins[i], i == whichDigit ? LOW : HIGH);
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

void MySegDisplay::updateState(int8_t digit, uint8_t bits)
{
    state[digit] = bits;
}

void MySegDisplay::begin(pin_size_t ledDigits[DIGIT_COUNT], pin_size_t shrRclk, pin_size_t oe)
{
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
        pinMode(ledDigits[i], OUTPUT);
    }

    pinMode(PIN_SPI_MOSI, OUTPUT);
    pinMode(shrRclk, OUTPUT);
    pinMode(outputEnablePin, OUTPUT);
    pinMode(PIN_SPI_SCK, OUTPUT);

    for (int i = 0; i < DIGIT_COUNT; i++)
    {
        this->digitPins[i] = ledDigits[i];
    }
    
    this->shrRclk = shrRclk;
    this->outputEnablePin = oe;
    refresh();
}

void MySegDisplay::setNumber(int number, int decimalPoint)
{
    _number = number;
    _decPoint = decimalPoint;
    for (size_t i = 1; i <= DIGIT_COUNT; i++)
    {
        auto bits = digitBits(number % 10);
        if (decimalPoint == i - 1)
        {
            bits |= pointBits();
        }
        updateState(DIGIT_COUNT - i, bits);
        number /= 10;
    }
}

void MySegDisplay::setGlyph(int digit, uint8_t bits)
{
    state[digit] = bits;
}

void MySegDisplay::horizontalLine(int height)
{
    int bits;
    switch (height)
    {
    case 0:
        bits = 0b00010001;
        break;
    case 1:
        bits = 0b00000010;
        break;
    case 2:
        bits = 0b10000000;
        break;
    default:
        break;
    }
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
        updateState(i, bits);
    }
}
