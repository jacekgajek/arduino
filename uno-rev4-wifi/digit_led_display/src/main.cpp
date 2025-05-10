#include <Arduino.h>
#include <MySegDisplay.h>

#define PIN_BUTTON PIN_D1

#define LED_DIGIT_1 PIN_D2
#define LED_DIGIT_2 PIN_D4
#define LED_DIGIT_3 PIN_D5
#define LED_DIGIT_4 PIN_D3

#define SHR_SER PIN_D8
#define SHR_RCLK PIN_D9
#define SHR_SRCLK PIN_D10

MySegDisplay display;

void triggerButton();

void setup() {
    pin_size_t ledPins[] ={ LED_DIGIT_1, LED_DIGIT_2, LED_DIGIT_3, LED_DIGIT_4 } ;

    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    Serial.println("Hello Serial!");
    display.begin(ledPins, SHR_SER, SHR_RCLK, SHR_SRCLK);
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    attachInterrupt(PIN_BUTTON, triggerButton, RISING);

}

int x = 0;
int dot = 0;
int lastChange = 0;
bool run = true;

bool isPrime(int p)
{
    for (int i = p / 2; i > 1; i--)
    {
        if (p % i == 0)
        {
            return false;
        }
    }
    return true;
}

void nextPrime(int &p)
{
    do
    {
        p += 2;
    } while (!isPrime(p));
}

void loop()
{
    if (run && millis() - lastChange >= 800)
    {
        display.horizontalLine(x++ % 3);

        display.setGlyph(1, 0b10010010);
        lastChange = millis();
        // nextPrime(x);
        // if (x > 9999)
        // {
        //     x = 1;
        // }
        // display.setNumber(x, dot++ % 4);
    }
    display.refresh();
}

void triggerButton()
{
    run = !run;
}
