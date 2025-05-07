#include <Globals.h>

void decreaseDelay();
void increaseDelay();

void initPins()
{
    pinMode(LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
}

void initInterrupts() {
    attachInterrupt(digitalPinToInterrupt(BUTTON_UP), decreaseDelay, RISING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN), increaseDelay, RISING);
}
void readConfig() {

    EEPROM.get(EEPROM_DELAY_LOCATION, blinkDelay);

    if (blinkDelay < 0)
    {
        blinkDelay = 1000;
    }
}
void initSerial() {
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    Serial.println("Hello setup!");
}

void init() {
    initPins();
    initInterrupts();
    readConfig();
    initSerial();
}
