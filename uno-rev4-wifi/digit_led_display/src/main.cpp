#include <Arduino.h>
#include <MySegDisplay.h>

#define PIN_BUTTON PIN_D1

#define LED_DIGIT_1 PIN_D2
#define LED_DIGIT_2 PIN_D4
#define LED_DIGIT_3 PIN_D5
#define LED_DIGIT_4 PIN_D3

#define SHR_RCLK PIN_D10
#define PIN_OE PIN_D8

MySegDisplay display;

void triggerButton();

void setup() {
    pin_size_t ledPins[] ={ LED_DIGIT_1, LED_DIGIT_2, LED_DIGIT_3, LED_DIGIT_4 } ;

    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    Serial.println("Hello Serial!");
    display.begin(ledPins, SHR_RCLK, PIN_OE);
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    attachInterrupt(PIN_BUTTON, triggerButton, RISING);

}

int x = 1;
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
    if (x & 1 == 0) {
        x--;
    }
    do
    {
        p += 2;
    } while (!isPrime(p));
}

int updateIntervalMs = 1000;
int speed = 2;
void loop()
{
    if (run && millis() - lastChange >= updateIntervalMs)
    {
        // display.horizontalLine(x++ % 3);

        // // display.setGlyph(0, 0b10000000);
        // display.setGlyph(1, 0b10000000);
        // display.setGlyph(2, 0b10000000);
        // display.setGlyph(3, 0b10000000);

        lastChange = millis();
        // Serial.println(F("Calculating next prime..."));
        nextPrime(x);
        // Serial.print(F("Got "));
        // Serial.println(x);
        if (x > 9999)
        {
            x = 1;
        }
        display.setNumber(x, dot % 4);
        dot ++;
    }
    display.refresh();
}

void triggerButton()
{
    speed = (speed + 1) % 5;
    switch (speed)
    {
    case 0:
        updateIntervalMs = 1000;
        break;
    case 1:
        updateIntervalMs = 500;
        break;
    case 2:
        updateIntervalMs = 200;
        break;
    case 3:
        updateIntervalMs = 100;
        break;
    case 4:
        updateIntervalMs = 50;
        break;
    default:
        break;
    }

}
